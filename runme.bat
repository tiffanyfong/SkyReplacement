@echo off

REM generate the result pictures

src\Release\imgpro input\globos_de_colores.jpg output\globos_brighntess_0.5.jpg -brightness 0.5

src\Release\imgpro input\globos_de_colores.jpg output\globos_brighntess_1.0.jpg -brightness 1.0

src\Release\imgpro input\globos_de_colores.jpg output\globos_brighntess_1.5.jpg -brightness 1.5

src\Release\imgpro -svdTest
