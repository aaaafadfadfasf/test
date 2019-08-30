@set Target=ospsip
del /f /q %Target%.lnt

echo -i"../../../10-Common/include" > %Target%.lnt
echo -i"../include" >> %Target%.lnt

copy %Target%.lnt %Target%_include.lnt

"C:\lint\Lint-nt"   %Target%.vcxproj >> %Target%.lnt

"C:\lint\Lint-nt"   +ffn -i"c:/lint" std.lnt env-vc9.lnt %Target%.lnt >%Target%_pclint.txt