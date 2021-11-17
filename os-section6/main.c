#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * 复制源文件到目标文件，如果目标文件不存在则创建
 *
 * dst: 目标文件的路径
 * src: 源文件的路径
 */
void copy_to_file(char *dst, char *src) {
  printf("\033[33m复制文件:\t\033[32m%s\t\033[33m>>>\t\033[32m%s\033[0m\n", src,
         dst);
  FILE *src_file, *dst_file;

  // 打开源文件
  if ((src_file = fopen(src, "rb")) == NULL) {
    perror("源文件不存在");
    exit(1);
  }

  // 打开目标文件，如果不存在则创建
  dst_file = fopen(dst, "wb");

  // 创建缓冲区，读取源文件到缓冲区，再写入目标文件
  char buf[4];
  int len;
  while ((len = fread(buf, 1, 4, src_file)) > 0) {
    fwrite(buf, 1, len, dst_file);
  }

  fclose(src_file);
  fclose(dst_file);
}

/**
 * 复制源文件到目标文件夹中
 *
 * dst_dir: 目标文件夹的路径
 * src：源文件的路径
 */
void copy_to_dir(char *dst_dir, char *src) {
  // 如果目标文件夹结尾没有 "/"，为其添加
  char src_tmp[1024];
  strcpy(src_tmp, src);
  if (dst_dir[strlen(dst_dir) - 1] != '/') {
    strcat(dst_dir, "/");
  }

  // 从源路径中分割得到源文件名，添加到目标路径结尾
  char *p, src_file_name[1024];
  while ((p = strsep(&src, "/")) != NULL) {
    strcpy(src_file_name, p);
  }
  strcat(dst_dir, src_file_name);

  // 调用 copy_to_file 进行文件到文件的复制
  copy_to_file(dst_dir, src_tmp);
}

/**
 * 递归复制源文件夹的所有子文件夹和文件和目标文件夹中
 * dst: 目标文件夹的路径
 * src: 源文件夹的路径
 *
 */
void copy_r(char *dst, char *src) {

  // 从源路径中分割得到源文件夹名，添加到目标路径结尾，并创建目标文件夹
  char *pt, src_folder_name[1024], src_tmp[1024], dst_tmp[1024];
  strcpy(src_tmp, src);
  while (strcmp(pt = strsep(&src, "/"), "") != 0) {
    strcpy(src_folder_name, pt);
  }
  strcpy(dst_tmp, dst);
  strcat(dst_tmp, src_folder_name);
  strcat(dst_tmp, "/");
  mkdir(dst_tmp, 0755);
  printf("\033[33m创建目标文件夹: \033[32m%s\033[0m\n", dst_tmp);

  // 遍历源文件夹的所有子文件
  DIR *dir;
  struct dirent *p;
  dir = opendir(src_tmp);
  while ((p = readdir(dir)) != NULL) {
    char *file_name = p->d_name;

    // 排除 "." 和 ".." 文件夹
    if (strcmp(file_name, ".") != 0 && strcmp(file_name, "..") != 0) {

      // 将父路径添加到子文件名前构成完整路径
      char src_path[1024], dst_path[1024];
      strcpy(src_path, src_tmp);
      strcat(src_path, file_name);

      // 判断源文件夹的子路径是文件还是文件夹
      struct stat info;
      stat(src_path, &info);
      if (S_ISDIR(info.st_mode)) {

        // 如果是文件夹，为源文件的结尾添加 "/"
        strcat(src_path, "/");
        copy_r(dst_tmp, src_path);
      } else {

        // 如果是文件，为目标文件的结尾添加文件名
        strcpy(dst_path, dst_tmp);
        strcat(dst_path, file_name);

        // 创建子进程负责源文件到目标文件的复制
        pid_t pid = fork();
        if (pid == 0) {

          // 调用 copy_to_file 进行文件到文件的复制
          copy_to_file(dst_path, src_path);
          exit(0);
        }
      }
    }
  }
  closedir(dir);
}

int main(int argc, char **argv) {
  // 记录程序运行的开始时间
  long start, end;
  start = clock();

  if (argc != 3 && argc != 4) {
    printf(
        "\033[31m使用方法：\033[32mcp [-r] <source_file> <(target_dir / "
        "target_file)>\033[0m\n");
    return 0;
  }

  // 非递归的情况
  if (argc == 3) {
    char *src = argv[1];
    char *dst = argv[2];

    // 判断源路径是文件还是文件夹，如果是文件夹，提示并结束程序
    struct stat info;
    stat(src, &info);
    if (S_ISDIR(info.st_mode)) {
      printf(
          "\033[31m如果需要复制文件夹，请使用 \033[32m<-r>\033[31m "
          "命令\033[0m\n");
      return 0;
    }

    // 判断目标路径是文件还是文件夹，采用不同的策略
    stat(dst, &info);
    if (S_ISDIR(info.st_mode)) {
      copy_to_dir(dst, src);
    } else {
      copy_to_file(dst, src);
    }
  }

  if (argc == 4) {
    // 递归的情况
    if (strncmp(argv[1], "-r", 2) == 0) {
      char src[1024], dst[1024];
      // 如果路径结尾没有 "/"，为其添加
      strcpy(src, argv[2]);
      strcpy(dst, argv[3]);
      if (src[strlen(src) - 1] != '/') {
        strcat(src, "/");
      }
      if (dst[strlen(dst) - 1] != '/') {
        strcat(dst, "/");
      }

      // 如果目标文件夹不存在，提示并退出程序
      if (access(dst, 0)) {
        printf("\033[31m目标文件夹不存在\033[0m\n");
        return 0;
      }
      copy_r(dst, src);
    } else {
      printf(
          "\033[31m使用方法：\033[32mcp [-r] <source_file> <(target_dir / "
          "target_file)>\033[0m\n");
      return 0;
    }
  }

  // 等待所有子进程结束
  while (wait(NULL) != -1) {}

  // 记录结束时间，做差得到运行时间
  end = clock();
  printf("\033[33m运行时间：\033[32m%ld\033[0m\n", end - start);

  return 0;
}