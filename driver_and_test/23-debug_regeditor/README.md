### 使用自己制作的io调试工具 
	# ./regeditor_test.app w32 0x56000054 0x0f
	# ./regeditor_test.app w32 0x56000054 0x7f 
	# ./regeditor_test.app r32 0x56000054 
	00. [56000054] = 7f
	# ./regeditor_test.app w32 0x56000054 0x0f
	# ./regeditor_test.app r32 0x56000054 
	00. [56000054] = 0f

