#! /bin/bash

input_file=tas.sql
output_file=tas.sql.output

rm -rf $output_file

tblname=""
convert_tbl()
{
    # create table tblCuser
    datainfo="$1"
    name=$(printf "$datainfo" |cut -d' ' -f3)
    tblname_raw=$name
    name=$(printf "${name:3}" |tr 'a-z' 'A-Z')
    tblname=$name
    
    echo "#define T_${tblname} \"${tblname_raw}\""
}
convert_tbl_filed()
{
    # userName             varchar(64) not null
    datainfo="$1"
    name=$(printf "$datainfo" |cut -d' ' -f1)
    filedname_raw=$name
    name=$(printf "$name" |tr 'a-z' 'A-Z')
    filedname=$name
    
    echo "#define F_${tblname}_${filedname} \"${filedname_raw}\""
}

convert_table_and_filed_def()
{
    bNeedConvert="false"
    while true; do 
        read lineval; [ $? -ne 0 ] && break
        
        echo "$lineval" |grep -qP '^create table'
        if [ $? -eq 0 ]; then
            convert_tbl "$lineval" >> $output_file
            bNeedConvert="true"
            continue
        fi
        
        echo "$lineval" |grep -qP '^\);'
        if [ $? -eq 0 ]; then
            echo '' >> $output_file
            bNeedConvert="false"
            continue
        fi
        
        [ $bNeedConvert = "false" ] && continue
        echo "$lineval" |grep -qP '^\('
        [ $? -eq 0 ] && continue
        
        echo "$lineval" |grep -qP '^primary key'
        if [ $? -ne 0 ]; then
            convert_tbl_filed "$lineval" >> $output_file
            continue
        fi
    done <$input_file
}

convert_table_and_filed_def
