用法：
1. 在PC上编译:
   进入源码目录执行make命令即可生成show_file应用程序
   把show_file,MSYH.TTF,hz.txt,utf8.txt,utf16be.txt,utf16le.txt复制到开发板上

2. 在开发板上执行下列命令之一，即可在LCD上显示
./show_file -s 16 -h HZK16   -f ./MSYH.TTF hz.txt
./show_file -s 16 -h HZK16   -f ./MSYH.TTF utf8.txt
./show_file -s 16 -h HZK16   -f ./MSYH.TTF utf16be.txt
./show_file -s 16 -h HZK16   -f ./MSYH.TTF utf16le.txt

其他大小:
./show_file -s 24 -h HZK16   -f ./MSYH.TTF utf8.txt
./show_file -s 32 -h HZK16   -f ./MSYH.TTF utf8.txt


   在开发板的串口里输入n并回车可以显示下一页文字，输入u并回车可以显示上一页文字，输入q并回车可以退出
   
3. 注意: 新宋字体simsun不支持16点阵大小的字体，这是它的缺陷   
   