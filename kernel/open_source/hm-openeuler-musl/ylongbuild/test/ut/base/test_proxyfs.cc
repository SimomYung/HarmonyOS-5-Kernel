#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include "stub.h"
#include "gtest/gtest.h"
#include <sys/stat.h>

#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include "limits.h"
#include <sys/mman.h>
#include <sys/types.h>
#include "securec.h"

class st_test_fs : public testing::Test {
protected:
    static void SetUpTestSuite()
    {
        init(); /* STUB INIT */
    }

    static void TearDownTestSuite()
    {
        clearAllStub();
    }

    virtual void SetUp() {}

    virtual void TearDown() {}
};

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


TEST_F(st_test_fs, testcase1_mkdir)
{
    const char *dirPath = "/var/run/file/";
    const char *fileName = "/var/run/file/testfile1";
    int ret = -1;
    ret = mkdir(dirPath, 0666);
    EXPECT_EQ(ret, 0);
    int fd = open(fileName, O_CREAT | O_RDWR, 0660);
    EXPECT_NE(fd, 0);

    const char src[] = "test write\n";
    int len = strlen(src);
    ret = write(fd, (void *)src, len);
    EXPECT_EQ(ret, len);
    close(fd);
    char dst[64] = {0};
    fd = open(fileName, O_RDWR);
    ret = read(fd, dst, len);
    EXPECT_EQ(ret, len);
    EXPECT_STREQ(src, dst);

    close(fd);
    ret = rmdir(dirPath);//删除非空目录失败
    EXPECT_NE(ret, 0);
}

TEST_F(st_test_fs, testcase2_mkdir)
{
    const char *dirPath = "/var/run/dir/test/";
    int32_t ret = -1;
    ret = mkdir(dirPath, 0755); //dirPath invalid，mkdir只能创建一级目录
    EXPECT_EQ(ret, -1);
    ret = rmdir(dirPath);//删除非法目录失败
    EXPECT_NE(ret, 0);
}

TEST_F(st_test_fs, testcase3_rmdir)
{
    const char *dirPath = "/var/run/file2/";
    int32_t ret = -1;
    ret = mkdir(dirPath, S_IRWXU);
    EXPECT_EQ(ret, 0);

    ret = rmdir(dirPath);//删除空目录，rmdir成功
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_fs, testcase4_mkdir)
{
    char dirPath[PATH_MAX] = "/var/run/";
    int ret;
    char suffix[PATH_MAX];
    memset(suffix, 'm', PATH_MAX - 1);
    strcat(dirPath, suffix);
    dirPath[PATH_MAX - 1] = '\0';
    ret = mkdir(dirPath, 0660);
    EXPECT_NE(ret, 0);
    ret = rmdir(dirPath);
    EXPECT_NE(ret, 0);
}


TEST_F(st_test_fs, testcase1_rename)
{
    const char *dirPath = "/var/run/file3/";
    int32_t ret = -1;
    ret = mkdir(dirPath, S_IRWXU);
    EXPECT_EQ(ret, 0);
    const char *oldName = "/var/run/file3/testfile3";
    const char *newName = "/var/run/file3/testfile3_1";
    int32_t fd = open(oldName, O_CREAT | O_RDWR, 0660);
    EXPECT_NE(fd, 0);

    const char src[] = "test write testfile3\n";
    int len = strlen(src);
    ret = write(fd, (void *)src, len);
    EXPECT_EQ(ret, len);
    close(fd);
    ret = rename(oldName, newName);
    EXPECT_EQ(ret, 0);
    fd = open(newName, O_RDWR);
    EXPECT_NE(fd, 0);
    char dst[64];
    ret = read(fd, dst, len);
    EXPECT_EQ(ret, len);
    close(fd);
    EXPECT_STREQ(src, dst);
}

TEST_F(st_test_fs, testcase2_rename)
{
    int32_t ret = -1;
    const char *oldName = "/var/run/file3/testfile4";
    const char *newName = "/var/run/file/testfile4_1";

    int32_t fd = open(oldName, O_CREAT | O_RDWR, 0660);
    EXPECT_NE(fd, 0);
    const char src[] = "test write testfile4\n";
    int len = strlen(src);
    ret = write(fd, (void *)src, len);
    EXPECT_EQ(ret, len);
    close(fd);
    ret = rename(oldName, newName);
    EXPECT_EQ(ret, 0);
    fd = open(newName, O_RDWR);
    EXPECT_NE(fd, 0);
    char dst[64];
    ret = read(fd, dst, len);
    EXPECT_EQ(ret, len);
    close(fd);
    EXPECT_STREQ(src, dst);
}

TEST_F(st_test_fs, testcase3_rename)
{
    int ret = -1;
    const char *oldDir = "/var/run/file5/";
    const char *newDir = "/var/run/file5_1/";
    const char *fileName = "/var/run/file5/testfile5";
    const char *newFile = "/var/run/file5_1/testfile5";
    ret = mkdir(oldDir, S_IRWXU);
    EXPECT_EQ(ret, 0);

    int fd = open(fileName, O_CREAT | O_RDWR, 0660);
    EXPECT_NE(fd, 0);

    const char src[] = "test write testfile5\n";
    int len = strlen(src);
    ret = write(fd, (void *)src, len);
    EXPECT_EQ(ret, len);
    close(fd);
    ret = rename(oldDir, newDir);
    EXPECT_EQ(ret, 0);
    fd = open(newFile, O_RDWR);
    EXPECT_NE(fd, 0);
    char dst[64];
    ret = read(fd, dst, len);
    EXPECT_EQ(ret, len);
    close(fd);
    EXPECT_STREQ(src, dst);
}

TEST_F(st_test_fs, testcase4_rename)
{
    char oldPath[PATH_MAX] = "/var/run/";
    char newPath[PATH_MAX];
    int ret;
    char suffix[PATH_MAX];
    memset(suffix, 'n', PATH_MAX - 1);
    strcat(oldPath, suffix);
    oldPath[PATH_MAX - 1] = '\0';
    memset(suffix, 'm', PATH_MAX - 1);
    strcat(newPath, suffix);
    newPath[PATH_MAX - 1] = '\0';

    ret = rename(oldPath, newPath);
    EXPECT_NE(ret, 0);
}

// test stat
TEST_F(st_test_fs, test_stat)
{
    const char *fileName = "/var/run/file/teststat";
    int32_t ret = -1;
    struct stat buf = {0};
    int32_t fd = open(fileName, O_CREAT | O_RDWR, 0660);
    EXPECT_NE(fd, 0);
    const char src[] = "test write teststat\n";
    int len = strlen(src);
    ret = write(fd, (void *)src, len);
    EXPECT_EQ(ret, len);
    ret = stat(fileName, &buf);
    EXPECT_EQ(ret, 0);
    EXPECT_EQ(len, buf.st_size);
    close(fd);
    ret = remove(fileName);
    EXPECT_EQ(ret, 0);
}

// test lseek
TEST_F(st_test_fs, test_lseek)
{
    const char *fileName = "/var/run/testlseek";
    int fd = open(fileName, O_CREAT | O_RDWR, 0660);
    EXPECT_GE(fd, 0);

    const char src[] = "test lseek\n";
    int len = strlen(src);
    int ret = write(fd, (void *)src, len);
    EXPECT_EQ(ret, len);
    long res = lseek(fd, 0, SEEK_SET);
    EXPECT_NE(res, -1);
    char dst[64] = {0};
    ret = read(fd, dst, len);
    EXPECT_EQ(ret, len);
    EXPECT_STREQ(src, dst);
    close(fd);
    ret = remove(fileName);
    EXPECT_EQ(ret, 0);
}

// test access
TEST_F(st_test_fs, testcase_access)
{
    const char *fileName = "/var/run/testaccess";
    int ret;
    int fd = open(fileName, O_CREAT | O_RDWR, 0660);
    EXPECT_NE(fd, -1);
    ret = access(fileName, 0640);
    EXPECT_NE(ret, 0);
    close(fd);
    ret = remove(fileName);
    EXPECT_EQ(ret, 0);
}

// test unlink
TEST_F(st_test_fs, testcase_unlink)
{
    const char *fileName = "/var/run/testunlink";
    int ret;
    int fd = open(fileName, O_CREAT | O_RDWR, 0660);
    EXPECT_NE(fd, -1);
    ret = unlink(fileName);
    EXPECT_EQ(ret, 0);
}


TEST_F(st_test_fs, testcase1_open)
{
    const char *fileName = " ";//打开非法文件名
    int fd = open(fileName, O_RDWR);
    EXPECT_EQ(fd, -1);
}

TEST_F(st_test_fs, testcase2_open)
{
    const char *fileName = "/var/run/file/";//打开目录
    int fd = open(fileName, O_RDWR);
    EXPECT_EQ(fd, -1);
}

TEST_F(st_test_fs, testcase3_open)
{
    int ret;
    const char *fileName = "/var/run/testopen3";
    int fd = open(fileName, O_RDWR | O_CREAT, 0666);
    EXPECT_NE(fd, -1);

    close(fd);
    fd = open(fileName, O_RDWR | O_CREAT | O_EXCL, 0600);
    EXPECT_EQ(fd, -1);
    ret = remove(fileName);
    EXPECT_EQ(ret, 0);
}

// max path len 255
TEST_F(st_test_fs, testcase4_open)
{
    char longFileName[PATH_MAX] = "/var/run/";
    int ret;
    char fileName[PATH_MAX];
    memset(fileName, 'a', PATH_MAX - 20);
    strcat(longFileName, fileName);
    int fd = open(longFileName, O_RDWR | O_CREAT, 0660);
    EXPECT_EQ(fd, -1);
    char src[] = "test maxlen of path name\n";
    int len = strlen(src);
    ret = write(fd, src, len);
    EXPECT_NE(ret, len);
    lseek(fd, 0, SEEK_SET);
    char dst[PATH_MAX];
    ret = read(fd, dst, len);
    EXPECT_NE(ret, len);
    close(fd);
}

TEST_F(st_test_fs, testcase5_open)
{
    char longFileName[PATH_MAX] = "/var/run/";
    int ret;
    char fileName[PATH_MAX];
    memset(fileName, 'c', PATH_MAX - 1);
    strcat(longFileName, fileName);
    int fd = open(longFileName, O_RDWR | O_CREAT, 0660);
    EXPECT_EQ(fd, -1);
    ret = access(longFileName, 0);
    EXPECT_NE(ret, 0);
    ret = unlink(longFileName);
    EXPECT_NE(ret, 0);
}


#define BUFSIZE 100
TEST_F(st_test_fs, testcase1_write)
{
    int ret;
    char buf[BUFSIZE];
    const char *fileName = "/var/run/testwrite1";

    memset(buf, 'w', BUFSIZE);
    int fd = open(fileName, O_RDWR | O_CREAT, 0660);
    int i;
    for (i = BUFSIZE; i > 0; i--) {
        ret = write(fd, buf, i);
        EXPECT_EQ(ret, i);
    }
    close(fd);
    ret = remove(fileName);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_fs, testcase2_write)
{
    int ret;
    const char *fileName = "/var/run/testwrite2";
    int fd = open(fileName, O_RDWR | O_CREAT, 0660);
    EXPECT_NE(fd, -1);

    ret = write(fd, NULL, BUFSIZE);
    EXPECT_EQ(ret, -1);//从NULL中读不到内容，会返回0字节
    close(fd);
    ret = remove(fileName);
    EXPECT_EQ(ret, 0);
}

TEST_F(st_test_fs, testcase3_write)
{
    int ret;
    const char *fileName = "/var/run/testwrite3";
    char wbuf[BUFSIZE];
    memset(wbuf, 'w', BUFSIZE - 1);

    int fd = open(fileName, O_RDWR | O_CREAT, 0660);
    EXPECT_NE(fd, -1);

    ret = write(fd, wbuf, BUFSIZE + 10);
    EXPECT_EQ(ret, BUFSIZE + 10);
    close(fd);
    ret = remove(fileName);
    EXPECT_EQ(ret, 0);
}


TEST_F(st_test_fs, testcase1_read)
{
    int ret;
    const char *fileName = "/var/run/testread1";
    int fd = open(fileName, O_RDWR | O_CREAT, 0660);
    EXPECT_NE(fd, -1);

    char wbuf[BUFSIZE], rbuf[BUFSIZE];
    void *addr;
    addr = mmap(0, 1, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    EXPECT_NE(addr, (void *)NULL);
    memset(wbuf, '0', BUFSIZE);

    ret = write(fd, wbuf, BUFSIZE);
    EXPECT_EQ(ret, BUFSIZE);

    fd = open(fileName, O_RDONLY);
    EXPECT_NE(fd, -1);

    // 从fd 往 addr读内容，即往addr写内容
    ret = read(fd, addr, BUFSIZE);
    EXPECT_EQ(ret, -1);

    memset(rbuf, '0', BUFSIZE);
    ret = read(fd, rbuf, BUFSIZE);
    EXPECT_EQ(ret, BUFSIZE);
    ret = memcmp(wbuf, rbuf, BUFSIZE);
    EXPECT_EQ(ret, 0);
    close(fd);
}

TEST_F(st_test_fs, testcase2_read)
{
    int ret;
    const char *fileName = "/var/run/testread3";
    int fd = open(fileName, O_RDWR | O_CREAT, 0660);
    EXPECT_NE(fd, -1);

    char wbuf[BUFSIZE], rbuf[BUFSIZE];
    void *addr;
    addr = mmap(0, 1, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0); //addr无权限
    EXPECT_NE(addr, (void *)NULL);
    memset(wbuf, '0', BUFSIZE);

    ret = write(fd, wbuf, BUFSIZE);
    EXPECT_EQ(ret, BUFSIZE);

    ret = write(fd, addr, BUFSIZE);
    EXPECT_NE(ret, BUFSIZE);

    fd = open(fileName, O_RDONLY);
    EXPECT_NE(fd, -1);
    memset(rbuf, 0, BUFSIZE);
    ret = read(fd, rbuf, BUFSIZE);
    EXPECT_EQ(ret, BUFSIZE);
    ret = memcmp(wbuf, rbuf, BUFSIZE);
    EXPECT_EQ(ret, 0);
    close(fd);
}

TEST_F(st_test_fs, testcase3_read)
{
    int ret;
    const char *fileName = "/var/run/testwrite4";
    char rbuf[BUFSIZE];
    char wbuf[BUFSIZE];
    int fd = open(fileName, O_RDWR | O_CREAT, 0660);
    EXPECT_NE(fd, -1);

    memset(wbuf, 'w', BUFSIZE);
    ret = write(fd, wbuf, BUFSIZE);
    EXPECT_EQ(ret, BUFSIZE);
    lseek(fd, 0, SEEK_SET);
    ret = read(fd, rbuf, BUFSIZE + 10);
    EXPECT_EQ(ret, BUFSIZE);
    close(fd);
    ret = remove(fileName);
    EXPECT_EQ(ret, 0);
}
