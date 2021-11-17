#! /bin/bash
#@ Author: Koorye
#@ Date: 2021-5-10
#@ Function: 检测文件大小 5 次并输出；如果文件大小改变，则重新开始检测

# 清屏
clear
read -p "输入文件名: " file_name

# 判断文件是否存在
if [ ! -f $file_name ]
then
  echo "找不到文件 $file_name，文件可能不存在或已经被移动"
  exit 0
else
  echo "文件 $file_name 存在，检测开始"
fi

# 使用 index 计数，表示检测次数
index=1

# 获取文件大小
file_size=$(/usr/bin/ls -l $file_name | cut -d " " -f 5)

# 当 index <= 5 时
while [ $index -le  5 ]
do
  # 获取新的文件大小
  new_file_size=$(/usr/bin/ls -l $file_name | cut -d " " -f 5)
  echo -e "第 $index 次检测\t文件名: $file_name\t文件大小: $file_size"

  # 如果文件大小改变，则重新计数
  if [ $new_file_size != $file_size ]
  then
    echo "文件 $file_name 大小变化，重新检测"
    file_size=$new_file_size
    index=1
    continue
  fi

  # 延时，index + 1
  sleep 5
  index=`expr $index + 1`
done

echo "文件检测结束"
exit 0