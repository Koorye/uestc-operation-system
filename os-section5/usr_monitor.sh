#! /bin/bash
#@ Author:   Koorye
#@ Date:     2021-5-17
#@ Function: 检测用户登录

# 展示所有登录的用户
#@ param: array 数组
function show_login_users() {
  echo "当前登录的用户: "
  for user in $@
  do
    echo -e "\033[32m$user\033[0m"
  done
}

# 判断数组是否包含元素
#@ param:  element 元素
#@ param:  array 数组
#@ return: 是否包含元素，1 表示包含，0 表示不包含
function contains() {
  element=$1
  array=($@)
  is_contained=false

  for ((i=0; i<${#array[@]}; i++))
  do
    if (( $i > 0 ))
    then
      if [ ${array[i]} == $element ]
      then
        return 1
      fi
    fi
  done
  return 0
}

# 获取所有登录的用户
username=$1
if [ -z $username ]
then
  echo -e "\033[31m使用方法: usr_monitor <username>\033[0m"
  exit 0
fi

index=1
while true
do
  clear
  echo "-------------------------------------------"
  echo -e "检测次数：\t$index"
  # 获取所有登录的用户
  user_array=(`who | cut -d " " -f 1`)
  show_login_users ${user_array[*]}

  contains $username ${user_array[*]}
  res=$?

  # 如果用户数组中包含 username
  if [ $res -eq 1 ]
  then
    echo -e "\033[31m用户 $username 已登录，退出检测\033[0m"
  # 如果用户数组中不包含 username
  elif [ $res -eq 0 ]
  then
    echo -e "\033[31m用户 $username 未登录\033[0m"
  fi

  echo -e "检测时间:\t" `date`
  echo "-------------------------------------------"
  if [ $res -eq 1 ]
  then
    exit 0
  fi

  index=`expr $index + 1`
  sleep 5
done