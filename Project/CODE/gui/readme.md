GUI in OLED
===========
define your menu topology in gui.cpp

子菜单树形图
============
- **BackGround**      *BasicLayout*
  - **HomePage**      *ListLayout*
    - **Debug**       *ListLayout*
    - **Steering**    *NoticeLayout*
    - **Attitude**    *ListLayout*
    - **Resistance**  *ListLayout*
    - **Speed**       *MenuLayout*
      - **Left**      *GraphLayout*
      - **Right**     *GraphLayout*
    - **ADC raw**     *ListLayout*
    - **PID**         *ListLayout*
  
规范
============
- 按需重写， 不写就默认空函数
```
   virtual void Show() = 0;
   virtual void KeyEnterPush(sched_event_data_t dat){}
   virtual void KeyUpPush(sched_event_data_t dat) {}
   virtual void KeyDownPush(sched_event_data_t dat) {}
   virtual void KeyLeftPush(sched_event_data_t dat) {}
   virtual void KeyRightPush(sched_event_data_t dat) {}
```

- *void UpdateValue* 从别的地方调用来更改要显示的数据
- **ListLayout** 树的子结点顺序要与 *SGUI_ITEMS_ITEM its[ChildrenCnt]* 相对应
- 有些 *Show()* 在第二层子类中定义，有些在第三层
- 每个构造函数都要传一个**tree**指针给基类，记录当前页面的子菜单有哪些
  


MDK常见错误详解集合
错误代码及错误信息 错误释义 
error 1: Out of memory 内存溢出 
error 2: Identifier expected 缺标识符 
error 3: Unknown identifier 未定义的标识符 
error 4: Duplicate identifier 重复定义的标识符 
error 5: Syntax error 语法错误 
error 6: Error in real constant 实型常量错误 
error 7: Error in integer constant 整型常量错误 
error 8: String constant exceeds line 字符串常量超过一行 
error 10: Unexpected end of file 文件非正常结束 
error 11: Line too long 行太长 
error 12: Type identifier expected 未定义的类型标识符 
error 13: Too many open files 打开文件太多 
error 14: Invalid file name 无效的文件名 
error 15: File not found 文件未找到 
error 16: Disk full 磁盘满 
error 17: Invalid compiler directive 无效的编译命令 
error 18: Too many files 文件太多 
error 19: Undefined type in pointer def 指针定义中未定义类型 
error 20: Variable identifier expected 缺变量标识符 
error 21: Error in type 类型错误 
error 22: Structure too large 结构类型太长 
error 23: Set base type out of range 集合基类型越界 
error 24: File components may not be files or objectsfile分量不能是文件或对象 
error 25: Invalid string length 无效的字符串长度 

error 26: Type mismatch 类型不匹配 
error 27：error 27：Invalid subrange base type 无效的子界基类型 
error 28：Lower bound greater than upper bound 下界超过上界 
error 29：Ordinal type expected 缺有序类型 
error 30：Integer constant expected 缺整型常量 
error 31：Constant expected 缺常量 
error 32：Integer or real constant expected 缺整型或实型常量 
error 33：Pointer Type identifier expected 缺指针类型标识符 
error 34：Invalid function result type 无效的函数结果类型 
error 35：Label identifier expected 缺标号标识符 
error 36：BEGIN expected 缺BEGIN 
error 37：END expected 缺END 
error 38：Integer expression expected 缺整型表达式 
error 39：Ordinal expression expected 缺有序类型表达式 
error 40：Boolean expression expected 缺布尔表达式 
error 41：Operand types do not match 操作数类型不匹配 
error 42：Error in expression 表达式错误 
error 43：Illegal assignment 非法赋值 
error 44：Field identifier expected 缺域标识符 
error 45：Object file too large 目标文件太大 
error 46：Undefined external 未定义的外部过程与函数 
error 47：Invalid object file record 无效的OBJ文件格式 
error 48：Code segment too large 代码段太长 
error 49：Data segment too large 数据段太长 
error 50：DO expected 缺DO 

error 51：Invalid PUBLIC definition 无效的PUBLIC定义 
error 52：Invalid EXTRN definition 无效的EXTRN定义 
error 53: Too many EXTRN definitions 太多的EXTRN定义 
error 54：OF expected 缺OF 
error 55：INTERFACE expected 缺INTERFACE 
error 56：Invalid relocatable reference 无效的可重定位引用 
error 57：THEN expected 缺THEN 
error 58：TO or DOWNTO expected 缺TO或DOWNTO 
error 59：Undefined forward 提前引用未经定义的说明 
error 61：Invalid typecast 无效的类型转换 
error 62：Division by zero 被零除 
error 63：Invalid file type 无效的文件类型 
error 64：Cannot read or write variables of this type 不能读写此类型变量 
error 65：Pointer variable expected 缺指针类型变量 
error 66：String variable expected 缺字符串变量 
error 67：String expression expected 缺字符串表达式 
error 68：Circular unit reference 单元UNIT部件循环引用 
error 69：Unit name mismatch 单元名不匹配 
error 70：Unit version mismatch 单元版本不匹配 
error 71：Internal stack overflow 内部堆栈溢出 
error 72：Unit file format error 单元文件格式错误 
error 73：IMPLEMENTATION expected 缺IMPLEMENTATION 
error 74：Constant and case types do not match 常量和CASE类型不匹配 
error 75：Record or object variable expected 缺记录或对象变量 

error 76：Constant out of range 常量越界 
error 77：File variable expected 缺文件变量 
error 78：Pointer expression expected 缺指针表达式 
error 79：Integer or real expression expected 缺整型或实型表达式 
error 80：Label not within current block 标号不在当前块内 
error 81：Label already defined 标号已定义 
error 82：Undefined label in preceding statement part 在前面未定义标号 
error 83：Invalid @ argument 无效的@参数 
error 84：UNIT expected 缺UNIT 
error 85: ";" expected 缺“；” 
error 86： ":" expected 缺“：” 
error 87： "," expected 缺“，” 
error 88： "(" expected 缺“（” 
error 89： ")" expected 缺“）” 
error 90： "=" expected 缺“=” 
error 91： ":=" expected 缺“:=” 
error 92： "[" or "(." Expected 缺“[”或“（.” 
error 93: "]" or ".)" expected 缺“］”或“.）” 
error 94： "." expected 缺“.” 
error 95: ".." expected 缺“..” 
error 96：Too many variables 变量太多 
error 97：Invalid FOR control variable 无效的FOR循环控制变量 
error 98：Integer variable expected 缺整型变量 
error 99：Files and procedure types are not allowed here 该处不允许文件和过程类型 
error 100：String length mismatch 字符串长度不匹配 

error 101：Invalid ordering of fields 无效域顺序 
error 102：String constant expected 缺字符串常量 
error 103：Integer or real variable expected 缺整型或实型变量 
error 104：Ordinal variable expected 缺有序类型变量 
error 105：INLINE error INLINE错误 
error 106：Character expression expected 缺字符表达式 
error 107：Too many relocation items 重定位项太多 
error 108：Overflow in arithmetic operation 算术运算溢出 
error 112：CASE constant out of range CASE常量越界 
error 113：Error in statement 表达式错误 
error 114：Cannot call an interrupt procedure 不能调用中断过程 
error 116：Must be in 8087 mode to compile this 必须在8087模式编译 
error 117：Target address not found 找不到目标地址 
error 118：Include files are not allowed here 该处不允许INCLUDE文件 
error 119：No inherited methods are accessible here 该处继承方法不可访问 
error 121：Invalid qualifier 无效的限定符 
error 122：Invalid variable reference 无效的变量引用 
error 123：Too many symbols 符号太多 
error 124：Statement part too large 语句体太长 

error 126：Files must be var parameters 文件必须是变量形参 
error 127：Too many conditional symbols 条件符号太多 
error 128：Misplaced conditional directive 条件指令错位 
error 129：ENDIF directive missing 缺ENDIF指令 
error 130：Error in initial conditional defines 初始条件定义错误 
error 131：Header does not match previous definition 和前面定义的过程或函数不匹配 
error 133：Cannot evaluate this expression 不能计算该表达式 
error 134：Expression incorrectly terminated 表达式错误结束 
error 135：Invalid format specifier 无效格式说明符 
error 136：Invalid indirect reference 无效的间接引用 
error 137：Structured variables are not allowed here 该处不允许结构变量 
error 138：Cannot evaluate without System unit 没有System单元不能计算 
error 139：Cannot access this symbol 不能存取符号 
error 140：Invalid floating point operation 无效的符号运算 
error 141：Cannot compile overlays to memory 不能编译覆盖模块至内存 
error 142：Pointer or procedural variable expected 缺指针或过程变量 
error 143：Invalid procedure or function reference 无效的过程或函数调用 
error 144：Cannot overlay this unit 不能覆盖该单元 
error 146：File access denied 不允许文件访问 
error 147：Object type expected 缺对象类型 
error 148：Local object types are not allowed 不允许局部对象类型 
error 149：VIRTUAL expected 缺VIRTUAL 
error 150: Method identifier expected 缺方法标识符 

error 151：Virtual constructors are not allowed 不允许虚构造函数 
error 152：Constructor identifier expected 缺构造函数标识符 
error 153：Destructor identifier expected 缺析构函数标识符 
error 154：Fail only allowed within constructors 只能在构造函数内使用Fail标准过程 
error 155：Invalid combination of opcode and operands 操作数与操作符无效组合 
error 156：Memory reference expected 缺内存引用指针 
error 157：Cannot add or subtract relocatable symbols 不能加减可重定位符号 
error 158：Invalid register combination 无效寄存器组合 
error 159：286/287 instructions are not enabled 未激活286/287指令 
error 160：Invalid symbol reference 无效符号指针 
error 161：Code generation error 代码生成错误 
error 162：ASM expected 缺ASM 
error 166：Procedure or function identifier expected 缺过程或函数标识符 
error 167：Cannot export this symbol 不能输出该符号 
error 168：Duplicate export name 外部文件名重复 
error 169：Executable file header too large 可执行文件头太长 
error 170：Too many segments 段太多 



一、 运行错误信息 
运行错误分为四类：1-99为DOS错误；100-149为I/O错误，发生I/O后，如果使用了编译开关{$I+}，程序将终止执行，否则编译开关为{$I-}，程序继续执行，并由IOResult函数返回错误信息； 150-199为严重错误，200-255为致命错误，致命错误将立即终止程序执行。 

错误代码及错误信息 错误释义 

error 1：Invalid function number 无效函数号 
error 2：File not found 文件未找到 
error 3：Path not found 找不到路径 
error 4：Too many open files 打开文件太多 
error 5：File access denied 文件不允许访问 
error 6：Invalid file handle 无效文件句柄 
error 12：Invalid file access code 无效文件存取代码 
error 15：Invalid drive number 无效驱动器号 
error 16：Cannot remove current directory 不能删除当前目录 
error 17：Cannot rename across drives 不能跨驱动器改名 
error 18：No more files 没有文件了 
error 100：Disk read error 读磁盘错 
error 101：Disk write error 写磁盘错 
error 102：File not assigned 文件变量未赋值 
error 103：File not open 文件未打开 
error 104：File not open for input 文件未打开为写方式 
error 105：File not open for output 文件未打开为读方式 
error 106：Invalid numeric format 无效数值格式 
error 150：Disk is write-protected 磁盘写保护 
error 151：Bad drive request struct length 错误的驱动器请求结构长度 
error 152：Drive not ready 驱动器未准备好 
error 154：CRC error in data 数据CRC校验错 
error 156：Disk seek error 磁盘定位错 
error 157：Unknown media type 未知的介质类型 
error 158：Sector Not Found 找不到扇区 
error 159：Printer out of paper 打印机缺纸 
error 160：Device write fault 设备写错误 
error 161：Device read fault 设备读错误 
error 162：Hardware failure 硬件故障 
error 200：Division by zero 被零除 
error 201：Range check error 范围检查出错 
error 202：Stack overflow error 栈溢出 
error 203：Heap overflow error 堆溢出 
error 204：Invalid pointer operation 无效的指针操作 
error 205：Floating point overflow 浮点运算上溢 
error 206：Floating point underflow 浮点运算下溢 
error 207：Invalid floating point operation 无效的浮点运算 
error 208：Overlay manager not installed 未安装覆盖管理程序 
error 209：Overlay file read error 读覆盖文件错误 
error 210：Object not initialized 对象未初始化 
error 211：Call to abstract method 调用抽象方法 
error 212：Stream registration error 流注_册错误 
error 213：Collection index out of range 集合下标越界 
error 214：Collection overflow error 集合上溢 
error 215：Arithmetic overflow error 算术运算上溢 
error 216：General Protection fault 一般保护性错误

keil c编译器错误与解决方法 

1. Warning 280:’i’:unreferenced local variable 
说明局部变量i 在函数中未作任何的存取操作解决方法消除函数中i 变量的宣告 

2. Warning 206:’Music3’:missing function-prototype 
说明Music3( )函数未作宣告或未作外部宣告所以无法给其他函数调用 
解决方法将叙述void Music3(void)写在程序的最前端作宣告如果是其他文件的函数则要写 
成extern void Music3(void),即作外部宣告 

3. Compling :C:\8051\MANN.C 
Error:318:can’t open file ‘beep.h’ 
说明在编译C:\8051\MANN.C 程序过程中由于main.c 用了指令＃i nclude “beep.h”,但 
却找不到所致解决方法编写一个beep.h 的包含档并存入到c:\8051 的工作目录中 

4. Compling:C:\8051\LED.C 
Error 237:’LedOn’:function already has a body 
说明LedOn( )函数名称重复定义即有两个以上一样的函数名称 
解决方法修正其中的一个函数名称使得函数名称都是独立的 

5. ***WARNING 16:UNCALLED SEGMENT,IGNORED FOR OVERLAY PROCESS 
SEGMENT: ?PR?_DELAYX1MS?DELAY 
说明DelayX1ms( )函数未被其它函数调用也会占用程序记忆体空间解决方法去掉 
DelayX1ms( )函数或利用条件编译#if …..#endif,可保留该函数并不编译 

6. ***WARNING 6 :XDATA SPACE MEMORY OVERLAP 
FROM : 0025H 
TO: 0025H 
说明外部资料ROM 的0025H 重复定义地址 
解决方法外部资料ROM 的定义如下Pdata unsigned char XFR_ADC _at_0x25 其中XFR_ADC 
变量的名称为0x25,请检查是否有其它的变量名称也是定义在0x25 处并修正它 

7. WARNING 206:’DelayX1ms’: missing function-prototype 
C:\8051\INPUT.C 
Error 267 :’DelayX1ms ‘:requires ANSI-style prototype C:\8051\INPUT.C 
说明程序中有调用DelayX1ms 函数但该函数没定义即未编写程序内容或函数已定义但未作 
宣告 
解决方法编写DelayX1ms 的内容编写完后也要作宣告或作外部宣告可在delay.h 的包含档 
宣告成外部以便其它函数调用 

8. ***WARNING 1:UNRESOLVED EXTERNAL SYMBOL 
SYMBOL:MUSIC3 
MODULE:C:\8051\MUSIC.OBJ(MUSIC) 
***WARNING 2:REFERENCE MADE TO UNRESOLVED EXTERNAL 
SYMBOL:MUSIC3 
MODULE:C:\8051\MUSIC.OBJ(MUSIC) 
ADDRESS:0018H 
说明程序中有调用MUSIC 函数但未将该函数的含扩档C 加入到工程档Prj 作编译和连接 
解决方法设MUSIC3 函数在MUSIC C 里将MUSIC C 添加到工程文件中去 

9. ***ERROR 107:ADDESS SPACE OVERFLOW 
SPACE: DATA 
SEGMENT: _DATA_GOUP_ 
LENGTH: 0018H 
***ERROR 118: REFERENCE MADE TO ERRONEOUS EXTERNAL 
SYMBOL: VOLUME 
MODULE: C:\8051\OSDM.OBJ (OSDM) 
ADDRESS: 4036H 
说明data 存储空间的地址范围为0~0x7f,当公用变量数目和函数里的局部变量如果存 
储模式设为SMALL 则局部变量先使用工作寄存器R2~R7 作暂存当存储器不够用时则会 
以data 型别的空间作暂存的个数超过0x7f 时就会出现地址不够的现象 
解决方法将以data 型别定义的公共变量修改为idata 型别的定义 

10. ***WARNING L15: MULTIPLE CALL TO SEGMENT 
SEGMENT: ?PR?_WRITE_GMVLX1_REG?D_GMVLX1 
CALLER1: ?PR?VSYNC_INTERRUPT?MAIN 
CALLER2: ?C_C51STARTUP 
***WARNING L15: MULTIPLE CALL TO SEGMENT 
SEGMENT: ?PR?_SPI_SEND_WORD?D_SPI 
CALLER1: ?PR?VSYNC_INTERRUPT?MAIN 
CALLER2: ?C_C51STARTUP 
***WARNING L15: MULTIPLE CALL TO SEGMENT 
SEGMENT: ?PR?SPI_RECEIVE_WORD?D_SPI 
CALLER1: ?PR?VSYNC_INTERRUPT?MAIN 
CALLER2: ?C_C51STARTUP 
该警告表示连接器发现有一个函数可能会被主函数和一个中断服务程序(或者调用中断 
服务程序的函数)同时调用, 
或者同时被多个中断服务程序调用。 
出现这种问题的原因之一是这个函数是不可重入性函数,当该函数运行时它可能会被一 
个中断打断,从而使得结果发生变化 
并可能会引起一些变量形式的冲突(即引起函数内一些数据的丢失,可重入性函数在任何时 
候都可以被ISR 打断,一段时间后又可以 
运行,但是相应数据不会丢失)。 
原因之二是用于局部变量和变量(暂且这样翻译,arguments,[自变量,变元一数值,用于 
确定程序或子程序的值])的内存区被其他函数的内存区所覆盖,如果该函数被中断,则它的 
内存区就会 
被使用,这将导致其他函数的内存冲突。 
例如,第一个警告中函数WRITE_GMVLX1_REG 在D_GMVLX1.C 或者D_GMVLX1.A51 被定义, 
它被一个中断服务程序或者一个调用了中断 
服务程序的函数调用了,调用它的函数是VSYNC_INTERRUPT,在MAIN.C 中。 
解决方法： 
如果你确定两个函数决不会在同一时间执行(该函数被主程序调用并且中断被禁止),并 
且该函数不占用内存(假设只使用寄存器), 
则你可以完全忽略这种警告。 
如果该函数占用了内存,则应该使用连接器(linker)OVERLAY 指令将函数从覆盖分析 
(overlay analysis)中除去,例如： 
OVERLAY (?PR?_WRITE_GMVLX1_REG?D_GMVLX1 ! *) 
上面的指令防止了该函数使用的内存区被其他函数覆盖。如果该函数中调用了其他函数, 
而这些被调用在程序中其他地方也被调用, 
你可能会需要也将这些函数排除在覆盖分析(overlay analysis)之外。这种OVERLAY 指令能 
使编译器除去上述警告信息。 
如果函数可以在其执行时被调用,则情况会变得更复杂一些。这时可以采用以下几种方 
法： 
1. 主程序调用该函数时禁止中断,可以在该函数被调用时用#pragma disable 语句来实现禁 
止中断的目的。必须使用OVERLAY 指令将该函数 
从覆盖分析中除去。 
2. 复制两份该函数的代码,一份到主程序中,另一份复制到中断服务程序中。 
3. 将该函数设为重入型。例如： 
void myfunc(void) reentrant { 
... 
} 
这种设置将会产生一个可重入堆栈,该堆栈被被用于存储函数值和局部变量,用这种 
方法时重入堆栈必须在STARTUP.A51 文件中配置。 
这种方法消耗更多的RAM 并会降低重入函数的执行速度。 

11. *** WARNING L16: UNCALLED SEGMENT, IGNORED FOR OVERLAY PROCESS 
SEGMENT: ?PR?_COMPARE?TESTLCD 
说明：程序中有些函数（或片段）以前（调试过程中）从未被调用过,或者根本没有 
调用它的语句。 
这条警告信息前应该还有一条信息指示出是哪个函数导致了这一问题。只要做点简 
单的调整就可以。不理它也没什么大不了的。 
解决方法:去掉COMPARE()函数或利用条件编译#if …..#endif,可保留该函数并不 
编译。 

----------------------------------------------------------------------------------------------------- 
C 编译器错误信息中文翻译 
Ambiguous operators need parentheses 不明确的运算需要用括号括起 
Ambiguous symbol ``xxx`` 不明确的符号 
Argument list syntax error 参数表语法错误 
Array bounds missing 丢失数组界限符 
Array size toolarge 数组尺寸太大 
Bad character in paramenters 参数中有不适当的字符 
Bad file name format in include directive 包含命令中文件名格式不正确 
Bad ifdef directive synatax 编译预处理ifdef 有语法错 
Bad undef directive syntax 编译预处理undef 有语法错 
Bit field too large 位字段太长 
Call of non-function 调用未定义的函数 
Call to function with no prototype 调用函数时没有函数的说明 
Cannot modify a const object 不允许修改常量对象 
Case outside of switch 漏掉了case 语句 
Case syntax error Case 语法错误 
Code has no effect 代码不可述不可能执行到 
Compound statement missing{ 分程序漏掉"{" 
Conflicting type modifiers 不明确的类型说明符 
Constant expression required 要求常量表达式 
Constant out of range in comparison 在比较中常量超出范围 
Conversion may lose significant digits 转换时会丢失意义的数字 
Conversion of near pointer not allowed 不允许转换近指针 
Could not find file ``xxx`` 找不到XXX 文件 
Declaration missing ; 说明缺少"；" 
Declaration syntax error 说明中出现语法错误 
Default outside of switch Default 出现在switch 语句之外 
Define directive needs an identifier 定义编译预处理需要标识符 
Division by zero 用零作除数 
Do statement must have while Do-while 语句中缺少while 部分 
Enum syntax error 枚举类型语法错误 
Enumeration constant syntax error 枚举常数语法错误 
Error directive :xxx 错误的编译预处理命令 
Error writing output file 写输出文件错误 
Expression syntax error 表达式语法错误 
Extra parameter in call 调用时出现多余错误 
File name too long 文件名太长 
Function call missing ) 函数调用缺少右括号 
Fuction definition out of place 函数定义位置错误 
Fuction should return a value 函数必需返回一个值 
Goto statement missing label Goto 语句没有标号 
Hexadecimal or octal constant too large16 进制或8 进制常数太大 
Illegal character ``x`` 非法字符x 
Illegal initialization 非法的初始化 
Illegal octal digit 非法的8 进制数字 
Illegal pointer subtraction 非法的指针相减 
Illegal structure operation 非法的结构体操作 
Illegal use of floating point 非法的浮点运算 
Illegal use of pointer 指针使用非法 
Improper use of a typedefsymbol 类型定义符号使用不恰当 
In-line assembly not allowed 不允许使用行间汇编 
Incompatible storage class 存储类别不相容 
Incompatible type conversion 不相容的类型转换 
Incorrect number format 错误的数据格式 
Incorrect use of default Default 使用不当 
Invalid indirection 无效的间接运算 
Invalid pointer addition 指针相加无效 
Irreducible expression tree 无法执行的表达式运算 
Lvalue required 需要逻辑值0 或非0 值 
Macro argument syntax error 宏参数语法错误 
Macro expansion too long 宏的扩展以后太长 
Mismatched number of parameters in definition 定义中参数个数不匹配 
Misplaced break 此处不应出现break 语句 
Misplaced continue 此处不应出现continue 语句 
Misplaced decimal point 此处不应出现小数点 
Misplaced elif directive 此处不应出现编译预处理elif 
Misplaced else 此处不应出现else 
Misplaced else directive 此处不应出现编译预处理else 
Misplaced endif directive 此处不应出现编译预处理endif 
Must be addressable 必须是可以编址的 
Must take address of memory location 必须存储定位的地址 
No declaration for function ``xxx`` 没有函数xxx 的说明 
No stack 缺少堆栈 
No type information 没有类型信息 
Non-portable pointer assignment 不可移动的指针（地址常数）赋值 
Non-portable pointer comparison 不可移动的指针（地址常数）比较 
Non-portable pointer conversion 不可移动的指针（地址常数）转换 
Not a valid expression format type 不合法的表达式格式 
Not an allowed type 不允许使用的类型 
Numeric constant too large 数值常太大 
Out of memory 内存不够用 
Parameter ``xxx`` is never used 参数xxx 没有用到 
Pointer required on left side of -> 符号->的左边必须是指针 
Possible use of ``xxx`` before definition 在定义之前就使用了xxx（警告） 
Possibly incorrect assignment 赋值可能不正确 
Redeclaration of ``xxx`` 重复定义了xxx 
Redefinition of ``xxx`` is not identical xxx 的两次定义不一致 
Register allocation failure 寄存器定址失败 
Repeat count needs an lvalue 重复计数需要逻辑值 
Size of structure or array not known 结构体或数给大小不确定 
Statement missing ; 语句后缺少"；" 
Structure or union syntax error 结构体或联合体语法错误 
Structure size too large 结构体尺寸太大 
Sub scripting missing ] 下标缺少右方括号 
Superfluous & with function or array 函数或数组中有多余的"&" 
Suspicious pointer conversion 可疑的指针转换 
Symbol limit exceeded 符号超限 
Too few parameters in call 函数调用时的实参少于函数的参数 
Too many default cases Default 太多(switch 语句中一个) 
Too many error or warning messages 错误或警告信息太多 
Too many type in declaration 说明中类型太多 
Too much auto memory in function 函数用到的局部存储太多 
Too much global data defined in file 文件中全局数据太多 
Two consecutive dots 两个连续的句点 
Type mismatch in parameter xxx 参数xxx 类型不匹配 
Type mismatch in redeclaration of ``xxx`` xxx 重定义的类型不匹配 
Unable to create output file ``xxx`` 无法建立输出文件xxx 
Unable to open include file``xxx`` 无法打开被包含的文件xxx 
Unable to open input file ``xxx`` 无法打开输入文件xxx 
Undefined label ``xxx`` 没有定义的标号xxx 
Undefined structure ``xxx`` 没有定义的结构xxx 
Undefined symbol ``xxx`` 没有定义的符号xxx 
Unexpected end of file in comment started on line xxx 从xxx 行开始的注解尚未结束文件不能结束 
Unexpected end of file in conditional started on line xxx 从xxx 开始的条件语句尚未结束文件不能结束 
Unknown assemble instruction 未知的汇编结构 
Unknown option 未知的操作 
Unknown preprocessor directive: ``xxx`` 不认识的预处理命令xxx 
Unreachable code 无路可达的代码 
Unterminated string or character constant 字符串缺少引号 
User break 用户强行中断了程序 
Void functions may not return a value Void 类型的函数不应有返回值 
Wrong number of arguments 调用函数的参数数目错 
``xxx`` not an argument xxx 不是参数 
``xxx`` not part of structure xxx 不是结构体的一部分 
xxx statement missing ( xxx 语句缺少左括号 
xxx statement missing ) xxx 语句缺少右括号 
xxx statement missing ; xxx 缺少分号 
xxx`` declared but never used 说明了xxx 但没有使用 
xxx`` is assigned a value which is never used 给xxx 赋了值但未用过 
Zero length structure 结构体的长度为零
