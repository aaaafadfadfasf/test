@set Target=3as
del /f /q %Target%.lnt

echo -i"../../../../10-Common/include" > %Target%.lnt
echo -i"../../../../10-Common/include/cbb" >> %Target%.lnt
echo -i"../../../../10-Common/include/cbb/dbx" >> %Target%.lnt
echo -i"../../../../10-Common/include/cbb/osp" >> %Target%.lnt
echo -i"../../../../10-Common/include/cbb/nms" >> %Target%.lnt
echo -i"../../../../10-Common/include/cbb/snmpadp" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/ospext" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/ospsip" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/tas" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/utility" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/commonconfig" >> %Target%.lnt
echo -i"../../../../10-Common/include/cms/dbix" >> %Target%.lnt

echo -i"../../code" >> %Target%.lnt
echo -i"../../code/config_util" >> %Target%.lnt
echo -i"../../code/data_sync" >> %Target%.lnt
echo -i"../../code/dbopr" >> %Target%.lnt
echo -i"../../code/tasks" >> %Target%.lnt
echo -i"../../code/data_sync" >> %Target%.lnt
echo -i"../../code/tasks/common" >> %Target%.lnt

copy %Target%.lnt %Target%_include.lnt

"C:\lint\Lint-nt"   %Target%.vcxproj >> %Target%.lnt

"C:\lint\Lint-nt"   +ffn -i"c:/lint" std.lnt env-vc9.lnt %Target%.lnt >%Target%_pclint.txt