SECTIONS
{
	.libdh.extensions : {
		__libdh_extpoint_start = .;
		*(.libdh.extensionpoints)
		__libdh_extpoint_end = .;
	}
	.libdh.extensions : {
		__libdh_plugin_start = .;
		*(.libdh.plugins)
		__libdh_plugin_end = .;
	}
}
INSERT AFTER .data;

SECTIONS
{
  .preinit_array    :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  }
}
INSERT AFTER .libdh.extensions;

