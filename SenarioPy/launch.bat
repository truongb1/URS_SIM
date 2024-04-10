@echo off
cls
echo Select which Python installation to use:
echo 1. parking_lot1.py      - Parallel parking and Ohio cone test
echo 2. parking_lot2.py      - Pull in left and right test
echo 3. highway_notraffic.py - Fast speed, multi lanes, no car
echo 4. highway_traffic.py   - Fast speed, multi lanes, ai car 
echo 5. sub_notraffic.py     - Medium speed, 2 lanes, no car
echo 6. sub_traffic.py       - Medium speed, 2 lanse, ai car 
echo 7. urban_notraffic      - Slow speed, suburb and city area, no car 
set /p choice="Enter your choice (1-7): 

if "%choice%"=="1" (
    set python_script=parking_lot1.py
) else if "%choice%"=="2" (
    set python_script=parking_lot2.py
) else if "%choice%"=="3" (
    set python_script=highway_notraffic.py
) else if "%choice%"=="4" (
    set python_script=highway_traffic.py
) else if "%choice%"=="5" (
    set python_script=sub_notraffic.py
) else if "%choice%"=="6" (
    set python_script=sub_traffic.py
) else if "%choice%"=="7" (
    set python_script=urban_notraffic.py
) else (
    echo Invalid choice. Exiting.
    timeout /t 3 /nobreak >nul
    exit /b
)

set "python_path=C:\Users\URS\Desktop\BeamPy\pythonProject"

echo Launching Python script %python_script% from %python_path%...
cd /d "%python_path%"
python.exe %python_script%
