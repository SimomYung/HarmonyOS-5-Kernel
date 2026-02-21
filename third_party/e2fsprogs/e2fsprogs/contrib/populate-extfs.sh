#!/bin/sh
#
# This script uses debugfs command to populate the ext2/3/4 filesystem
# from a given directory.
#
# Added mode, uid and gid config function by OpenHarmony team. 
export LANG=en_US.UTF-8
export LANGUAGE=en_US:en

do_usage () {
	cat << _EOF
Usage: populate-extfs.sh <source> <device> <cfgfile>
Create an ext2/ext3/ext4 filesystem from a directory or file

  source: The source directory or file
  device: The target device

_EOF
	exit 1
}

CFGFILE_CNT=5
FILEMODE_IDX=2
FILEU_IDX=3
FILEG_IDX=4
_NEW_FILEMODE=0
_NEW_FILEUID=0
_NEW_FILEGID=0

declare -a CONFIG_ARRAY

do_parsecfgfile () {
	i=0
	while read _BINFILE_ _FILETYPE_ _FILEMODE_ _FILEUID_ _FILEGID_
	do
		CONFIG_ARRAY[${i}]=${_BINFILE_}
		i=$[$i+1]
		CONFIG_ARRAY[${i}]=${_FILETYPE_}
		i=$[$i+1]
		CONFIG_ARRAY[${i}]=${_FILEMODE_}
		i=$[$i+1]
		CONFIG_ARRAY[${i}]=${_FILEUID_}
		i=$[$i+1]
		CONFIG_ARRAY[${i}]=${_FILEGID_}
		i=$[$i+1]
	done < ${CFGFILE}
}

do_getfilecfgmode () {
	i=0
	_NEW_FILEMODE_=$2
	_NEW_FILEUID_=0
	_NEW_FILEGID_=0
	while [ $i -lt ${#CONFIG_ARRAY[@]} ]
	do
		if [ "$1" == "${CONFIG_ARRAY[$i]}" ]; then
			# Change file mode from OCT to HEX.
			_NEW_FILEMODE_=$(echo "obase=16;$((0x$MODE & (~07777) | 0${CONFIG_ARRAY[$i+${FILEMODE_IDX}]}))"|bc)
			_NEW_FILEUID_=${CONFIG_ARRAY[$i+${FILEU_IDX}]}
			_NEW_FILLEGID_=${CONFIG_ARRAY[$i+${FILEG_IDX}]}
			return 0
		fi
		i=$[$i+${CFGFILE_CNT}]
	done
}

[ $# -ne 3 ] && do_usage

SRCDIR=${1%%/}
DEVICE=$2
CFGFILE=$3

# parse config file
do_parsecfgfile

# Find where is the debugfs command if not found in the env.
if [ -z "$DEBUGFS" ]; then
	CONTRIB_DIR=$(dirname $(readlink -f $0))
	DEBUGFS="$CONTRIB_DIR/../debugfs/debugfs"
	DEBUGFS=debugfs
fi

# "debugfs" command check.
command -v debugfs >/dev/null 2>&1 || \
{ echo >&2 "debugfs is required, please install by \"apt-get install e2fsprogs\". Aborting."; exit 1; }

{
	CWD="/"
	find $SRCDIR | while read FILE; do
                TGT="${FILE##*/}"
                DIR="${FILE#$SRCDIR}"
                DIR="${DIR%$TGT}"

		# Skip the root dir
		[ ! -z "$DIR" ] || continue
		[ ! -z "$TGT" ] || continue

		if [ "$DIR" != "$CWD" ]; then
			echo "cd $DIR"
			CWD="$DIR"
		fi

		# Only stat once since stat is a time consuming command
		STAT=$(stat -c "TYPE=\"%F\";DEVNO=\"0x%t 0x%T\";MODE=\"%f\";U=\"%u\";G=\"%g\"" $FILE)
		eval $STAT

		case $TYPE in
		"directory")
			echo "mkdir $TGT"
			;;
		"regular file" | "regular empty file")
			echo "write $FILE $TGT"
			;;
		"symbolic link")
			LINK_TGT=$(readlink $FILE)
			echo "symlink $TGT $LINK_TGT"
			;;
		"block special file")
			echo "mknod $TGT b $DEVNO"
			;;
		"character special file")
			echo "mknod $TGT c $DEVNO"
			;;
		"fifo")
			echo "mknod $TGT p"
			;;
		*)
			echo "Unknown/unhandled file type '$TYPE' file: $FILE" 1>&2
			;;
		esac

		do_getfilecfgmode ${FILE#$SRCDIR} $MODE $U $G
		MODE=${_NEW_FILEMODE_}
		U=${_NEW_FILEUID_}
		G=${_NEW_FILEGID_}
		# Set the file mode
		echo "sif $TGT mode 0x$MODE"

		# Set uid and gid
		echo "sif $TGT uid $U"
		echo "sif $TGT gid $G"
	done

	# Handle the hard links.
	# Save the hard links to a file, use the inode number as the filename, for example:
	# If a and b's inode number is 6775928, save a and b to /tmp/tmp.VrCwHh5gdt/6775928.
	INODE_DIR=`mktemp -d` || exit 1
	for i in `find $SRCDIR -type f -links +1 -printf 'INODE=%i###FN=%p\n'`; do
		eval `echo $i | sed 's$###$ $'`
		echo ${FN#$SRCDIR} >>$INODE_DIR/$INODE
	done
	# Use the debugfs' ln and "sif links_count" to handle them.
	for i in `ls $INODE_DIR`; do
		# The link source
		SRC=`head -1 $INODE_DIR/$i`
		# Remove the files and link them again except the first one
		for TGT in `sed -n -e '1!p' $INODE_DIR/$i`; do
			echo "rm $TGT"
			echo "ln $SRC $TGT"
		done
		LN_CNT=`cat $INODE_DIR/$i | wc -l`
		# Set the links count
		echo "sif $SRC links_count $LN_CNT"
	done
	rm -fr $INODE_DIR
} | $DEBUGFS -w -f - $DEVICE
