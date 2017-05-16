@echo off

REM generate the result pictures

src\Release\imgpro SkyVid\test0000001.jpg SkyVid\OUTPUTtest0000001.jpg -skyReplace newSky.jpg 10

src\Release\imgpro -svdTest
