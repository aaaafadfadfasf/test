@set Target=dbix
del /f /q %Target%.lnt

echo -i"../../../../10-Common/include" > %Target%.lnt
echo -i"../../../../10-Common/include/cms" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/ospext" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/ospsip" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/dbix" >> %Target%.lnt

echo -i"../../code" >> %Target%.lnt
echo -i"../../code/utils" >> %Target%.lnt

copy %Target%.lnt %Target%_include.lnt

"C:\lint\Lint-nt"   %Target%.vcxproj >> %Target%.lnt

"C:\lint\Lint-nt"   +ffn -i"c:/lint" std.lnt env-vc9.lnt %Target%.lnt >%Target%_pclint.txt