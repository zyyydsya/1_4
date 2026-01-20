@echo off
chcp 65001 >nul
echo ==============================================
echo          CCS仓库C代码格式化工具
echo ==============================================
echo.

:: 第一步：检查astyle是否可用
astyle --version >nul 2>&1
if errorlevel 1 (
    echo ❌ 错误：未找到astyle.exe，请先将astyle配置到系统PATH环境变量！
    pause
    exit /b 1
)
echo ✅ 检测到astyle工具已安装
echo.

:: 第二步：递归格式化所有.c/.h文件（匹配目标参数）
echo 📌 开始递归格式化当前目录及子目录的 .c/.h 文件...
echo.
powershell -Command "Get-ChildItem -Path . -Recurse -Include *.c,*.h | ForEach-Object { astyle.exe --style=allman -S -U -t -n -K -p -s4 -j -q -Y -xW -xV $_.FullName; Write-Host '✅ 已格式化：' $_.FullName -ForegroundColor Green }"

:: 第三步：执行完成提示
echo.
echo ==============================================
echo 📊 格式化任务执行完成！
echo 💡 请打开 .c 文件检查格式是否符合预期
echo ==============================================
pause