@set Target=cmu
del /f /q %Target%.lnt
del /f /q %Target%_include.lnt

echo -i"../../../10-Common/include" > %Target%.lnt
echo -i"../include" >> %Target%.lnt

copy %Target%.lnt %Target%_include.lnt

"C:\pclint\Lint-nt"   %Target%.vcxproj >> %Target%.lnt

"C:\pclint\Lint-nt"   +ffn -i"C:\pclint" std.lnt env-vc9.lnt %Target%.lnt >%Target%_pclint.txt