__asm(".global __use_no_semihosting\n\t");

//const char __stdin_name[150];
//const char __stdout_name[150];
//const char __stderr_name[150];
typedef int FILEHANDLE;

//重写标准库函数，这时printf、fopen、fclose等文件操作函数运行时就会调用你的重写函数，这些重写函数只是几个简单的例子，并没有重写所有的文件操作函数
void _sys_exit(int status) {
    // while(1);
}
FILEHANDLE _sys_open(const char *name, int openmode) { return 0; }

int _sys_close(FILEHANDLE fh) { return 0; }

int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len,
               int mode) {
    return 0;
}

int _sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode) {
    return 0;
}

int _sys_istty(FILEHANDLE fh) { return 0; }

int _sys_seek(FILEHANDLE fh, long pos) { return 0; }

int _sys_ensure(FILEHANDLE fh) { return 0; }

long _sys_flen(FILEHANDLE fh) { return 0; }

int _sys_tmpnam(char *name, int fileno, unsigned maxlength) { return 0; }

void _ttywrch(int ch) {}
int remove(const char *filename) { return 0; }

//char *_sys_command_string(char *cmd, int len) { return 0; }
