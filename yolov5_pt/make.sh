#!/bin/bash

# 删除 runs 文件夹中的所有内容
if [ -d "runs" ]; then
  rm -rf runs/*
  echo "已清空 runs 文件夹内容，但保留了 runs 文件夹本身。"
else
  echo "目录 runs 不存在。"
fi
