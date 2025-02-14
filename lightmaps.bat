@echo off
rem C:\Users\Poga\Desktop\painis\tram-sdk\devtools\tbsp\tbsp centraltirgus -worldspawn centraltirgus
rem C:\Users\Poga\Desktop\painis\tram-sdk\devtools\tmap\tmap centraltirgus 1024
rem C:\Users\Poga\Desktop\painis\tram-sdk\devtools\trad\trad centraltirgus centraltirgus-worldspawn 1024 1024 -pad 2

call :bake pool-water 128
call :bake chairs-hallway 128
call :bake gym-shelf 128
call :bake group-interior-a 128
call :bake group-interior-b 128
call :bake witch-barrier 128

rem C:\Users\Poga\Desktop\painis\tram-sdk\devtools\trad\trad level -pad 2


goto :eof

:bake
C:\Users\Poga\Desktop\painis\tram-sdk\devtools\tmap\tmap %~1 %~2
rem C:\Users\Poga\Desktop\painis\tram-sdk\devtools\trad\trad centraltirgus %~1 %~2 %~2 -worldspawn centraltirgus-worldspawn -pad 2
goto :eof
