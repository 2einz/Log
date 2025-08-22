#!/usr/bin/env bash
set -e

IGNORE_DIRS=("build" "third_party" "external")

# 构造 find 的排除规则（使用数组避免 eval）
find_args=()
for d in "${IGNORE_DIRS[@]}"; do
  find_args+=( -path "./$d" -prune -o )
done

# 执行 find 命令，用 -print0 处理特殊文件名，显式指定输出动作
find . "${find_args[@]}" -type f \( -name "*.h" -o -name "*.hpp" -o -name "*.c" -o -name "*.cpp" -o -name "*.cc" \) -print0 |
  while IFS= read -r -d '' f; do
    echo "Formatting $f"
    clang-format -i -style=file "$f"
  done

echo "🎉 格式化完成"