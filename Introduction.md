# SimpleGUI项目介绍
---

#### 起源
这套GUI系统其实来自于一个切实的需求。本人大学学习嵌入式，毕业至今依旧从事软件和嵌入式相关开发工作。但是我朝工资你懂的，基本不够花，所以在闲余时间会做一些兼职开发工作。之前的开发工作，很多都是直接一块板子几个灯，然后通过串口、USB或者蓝牙和手机或者电脑进行通讯，操作也基本通过手机和电脑等上位终端完成，人机交互基本不在设备上执行。但是去年接手的一个工控需求，要求终端机能够完成完整的交互，使用12864或者19264进行显示。老实说，我不喜欢，因为这样的复杂操作就需要GUI支持，而单片机上不可能像上位机那样，给你提供完整的操作库，而是需要开发者自己去一个像素一个像素的实现，有时候感觉甚至用超级终端都比这个来的省心。但客户就是上帝，于是我就走上了一条不归路。

#### 萌芽
最初我尝试使用emWin，库和范例代码我也下载研究了一下，还在加了一个专门玩儿emWin的群，看着群里大神们做的帅呆酷毙，以假乱真的GUI，我是一百个佩服，寻思着，我得啥时候达到这个段位。
但是慢慢的，我发现emWin在强大的背后有一些小问题。首先最要命的就是授权，用Keil编译还需要加一个License，至于用其他IDE我就不清楚了。对于我们这些开发者，尤其是业余开发者来说，要钱就是要命啊！就算破解了，谁知道什么时候会收到谁的律师函？！其次，强大的emWin对内存的消耗也是十分强大的，单片机的内存资源普遍匮乏，我也向群里的大神了解过，华丽的GUI背后需要的是外置存储、内存拓展和文件系统的支持，这无论是软件设计成本还是硬件设计成本，都是一笔不小的附加。
但是事实上，我这种简单设备的开发，并不需要如此华丽的外表，只需要能够支持基本的操作就可以，诸如图层、窗体等功能在大多数工业开发上很难用到，就算用到了，多半也都是彩屏了。于是我就想，难道就没有类似emWin这样但是专门针对单色屏的GUI么，省下来的资源可以做更大量的数据转储和复杂运算啊！然后我在网上搜索了一些资料，以及包括周立功的ZLGGUI的代码及实现，于是就萌生了自己制作GUI的想法。
因为之前提到的项目是使用的KS0108显示屏，所以这套系统的开发最早仅仅适配了KS0108，数据结构等也都是针对KS0108的工作模式设计的。后期也有研究过使用自带字库的ST7920，但是字库使用限制条件太多，很鸡肋，而且还更贵一些，所以就放弃了。
就这样，SimpleGUI原型设计诞生了。

#### 艰难的第一步
最开始攻克的课题是任意位置显示文字，网上所有的资料都是基于最基本的KS0108操作模式的文字显示，纵坐标必须是8的整数倍，我开始也是这样做的，使用8\*16的英文字符以及16\*16的中文字符，但是后来发现，外观很不理想，于是我的第一个课题就是以像素为单位任意位置显示任意大小的汉字。经过一周的努力，任意位置，任意字号的问题解决了，而且我发现，12像素的中文文字比16像素的好看多了，特别是绘制列表的时候，边上留上一像素的留白，比上顶天下顶地的好看多了！
>![SimpleGUI原型设计](https://images.gitee.com/uploads/images/2018/1029/234515_687c5c4c_769424.png "SimpleGUI-1.png")

#### 接口完善
为了完成这个项目，我就做了几个比较常用的接口，比如绘制点、线、矩形以及区域反色，然后以这些基础的绘图操，又衍生了列表，以及时间日期的调整框。然后我发现有些操作，特别是反显时，需要读点，屏幕刷新特别慢，看着特别闹心，于是我在单片机里开辟了一个1.5K的显示缓存（192\*8=1536），每个字节对应屏幕上一个页单元，所有的操作都在这里进行，另外有四个标记量标记被修改过的矩形部分，以实现局部刷新，显示效果终于比较满意了。
为了赶工期，其中有些代码还是将就出来的，离通用化还有一定的距离，但是其中不乏令我十分满意的设计，比如列表参数的引入，以及不同类型参数列表项的混排。

#### 发布与开源
SimpleGUI诞生时候，并没有SimpleGUI这个名字，因为当时我没有开源的打算，至于原因，要从很久以前说起了。
大学时候，以及大学毕业后的一段时间，那时候还在主力折腾51单片机，经常出没于某论坛（为避免恶意攻击之嫌，不点名），里面单片机社区很多就是就瓶装新酒的帖子，颠来倒去不断的仿制。这本来无可厚非，学习嘛，都是这样一步一步走来的。但是我个人就经常遇到一些很没品的伸手党，私信骚扰，讨要资料，不给就好象失了德行一样。更有甚者，设计资料共享到论坛上，个把月不到，一模一样的东西就能在淘宝上搜到了，美其名曰各种自主创新设计，技术细节一问三不知。
直到有一次，因为拒绝提供一个原创的设计资料，被人恶语相向，自此，在没有在网上发布过任何作品……
重新让我对开源燃起希望的，是另一个社区没那就是OSC开源中国，至于怎么一头撞进来的我记不太清楚了，但是进入之后，才发现之前抱怨的风气恶劣，是因为自己找错了环境和平台，这里才是开源共享和交流学习的地方，而且代码托管和迭代更新相比论坛共享也更专业更方便。
我不喜欢，向人伸手要东西，本着投桃报李的原则，我觉得把SimpleGUI投入OSC，算是纳个投名状吧。

#### 标准化
决定开源后，我将这个GUI系统重新整理，规范和追加一些常用接口，并留出足够的拓展空间，向“简约而不简单”的方向努力！
由于平时在家的时间不多，所以我悄悄的把之前的工程文件带到公司，开始删除一些实际的功能化代码，只留存界面绘图相关功能的代码，因为考虑到可能没时间写很详细的API说明，所以我给每一个接口都加上尽可能详尽的注释。由于楼主在一家日企工作，电脑都是日语的，上班干这些也是偷偷的干，而SimpleGUI的文字绘制函数是按照GB2312编码规则实现的，为避免乱码的尴尬问题，所有的注释都写成了英文，当然，后续代码完善后，将考虑更新成中文注释。
慢慢的，代码整理的差不多了，函数名、结构体名、变量名都已经尽可能的整理整齐，增强可读性，在19264的屏幕上试了试，效果不错！
至于SimpleGU这个名字，还要感谢OpenDev论坛的朋友，是他们给我的灵感，给予了SimpleGUI这个名字。

#### 走火入魔
在SimpleGUI的开发与调试过程中，我又发现了一个新问题。
最开始时候，由于没有编写片外数据读取接口，字库数据都被保存在片内，而编译好的包含GB2312字库的二进制文件体积很大，有200K以上，这还仅仅就是一级字库，往STM32里烧录需要大概30秒左右，有时候一点儿小改动想看看效果都要等上半分钟，对于我等急脾气来说，实在忍不了啊！
于是我就想起了emWin的那个用VisualStudio运行的模拟器，然后就有了大家接下来要看到的这个东西！
>![SimpleGUI模拟器](https://images.gitee.com/uploads/images/2018/1029/234614_a885f996_769424.png "SimpleGUI-2.png")
>![SimpleGUI模拟器](https://images.gitee.com/uploads/images/2018/1029/234634_b4f3110a_769424.png "SimpleGUI-3.png")

这是一款单色屏模拟器，前前后后折腾了有一段时间，使用wxWidgets开源GUI框架，在CodeBlocks IDE下开发。这个模拟器可以模拟绝大部分的显示和交互操作，方便脱离硬件进行GUI的开发。而且，这个模拟器我还增加了保存LCD截屏和复制到剪贴板功能，如果应用于项目，可以将编写好的GUI部分用模拟器运行，用于演示和说明书编写时的截图操作！
模拟环境同样开源，方便大家基于SimpleGUI进行开发。

#### 邪路
在模拟器开发到一半的时候，我突然想到了另一种东西，那就是串口屏，某宝有卖，PLC的屏幕也是这个原理。
我的模拟器都做到这份儿上了，难道就不能再进一步？如果有一根儿串口线，把电脑和单片机连起来，那么电脑能不能变成串口屏Plus？！
于是，又有了一款叫做LCDSimulator的软件，算是SimpleGUI的一个衍生产物，发布在这里，大伙儿可以去看一下！

>OpenDev论坛:http://www.openedv.com/forum.php?mod=viewthread&tid=106692&extra=

>Gitee托管:https://gitee.com/Polarix/lcdsimulator
    
#### 写在最后
其实这一路折腾下来挺累的，从开发到维护到改进，前前后后一年有余，从闹心，到小有成果，到系统一点儿点儿逐步建立，再推倒重写，在一点儿点儿建立。直到有了今天的成果。
这个东西我自己也会用到，分享出来也是想方便大家使用和学习，但是楼主也有自己的家庭和事业，所以更新频率无法保证，如果大家发现问题，可以跟帖回复或发送邮件到326684221@qq.com，本人一定在第一时间响应。
最后还有一点儿小感想。
首先，特别感谢wxWidgets技术群(104728878)各位的帮助。
在SimpleGUI发布前，刚和一个远在广西的朋友通过电话，其实是个网友，群里认识的，同为技术者，出来打点儿野食果腹。我们聊了一些自己最近研究的，以前研究的，研究一半儿搁置的……很多东西，突然发现，现在真的已经不是个人英雄的时代了，一个人再牛，也不可能样样精通，业务全覆盖，很多时候我们要借助别人的力量，但我们不是公司，我们借助的力量可能来自一个素未谋面的人，慢慢的，这种行为就变成了所谓“开源”，而在我的认识中，开源，就是一个大家学习、交流，取众人之长补己之短以致大成的过程。
有些时候，经常能看到有些人在别人的发布贴上回帖，说自己是小白，敬佩大神之类的。王侯将相，宁有种乎，谁都不是抱着开发板出生的，都是踩着自己敲的代码，一步一步爬向自己渴望的高度。当然，人各有所长，不能用一个标准去衡量所有人，但是，就像之前说的，开源是一个集百家之大成的过程，也许我个人代码编码能力不足，但是或许我能提供一个或一套很好看的图标，或者在使用公共库的时候，发现了一个BUG，并公布出来告诉所有使用者和开发者……关于开源，我们能做的其实很多，甚至有时候，开发者们需要的可能仅仅就是个脑洞而已。
开源不仅仅就是拿出来给大家玩玩，更是一种交流、学习、改进、创新的过程，开源软件中，更不乏可以比肩或超越同类收费软件的作品，在我看来，使用开源软件，可以省下的口袋里的银子，但与此同时，要多付一份责任。
以上所有言论只代表个人观点，不针对任何个人，如有冒犯，还请见谅。

致敬，每一位开源开发者。

#### 参与讨论
如果您有意参与SimpleGUI的讨论与改进，可以在以下地方留言：

1. 开源中国SimpleGUI页面：https://www.oschina.net/p/simplegui
2. 码云页面：https://gitee.com/Polarix/simplegui
3. QQ交流群：799501887