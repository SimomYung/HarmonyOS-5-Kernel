# 三方开源软件tex-hyphen

## tex-hyphen简介
tex-hyphen是一个用于TeX系统的断字模式库，它能够在多种语言中正确地将单词断行，以改善排版效果。

来源：tex-hyphen  
URL：https://github.com/hyphenation/tex-hyphen  
版本：CTAN-2021.03.21  
License：多种组合  

## 引入背景陈述
在多语言文档处理和排版中，正确的断字处理是至关重要的。tex-hyphen 提供了一套通用的断字模式，支持多种语言，为高质量的排版提供了基础保障。在OpenHarmony中，引入tex-hyphen可以显著提升多语言文档的排版质量。

## 语种归类
tex目录下包含了多个来自TeX hyphenations patterns的连字符规则，不同语种使用的开源许可证各不相同，整理归类：
* MIT License
* GPL,GPL 2
* LGPL 1,LGPL 2.1
* LPPL 1,LPPL 1.2,LPPL 1.3
* MPL 1.1
* BSD 3

OHOS中使用以下语种资源，以下语种均使用友好型开源协议：
* be - 白俄罗斯语（Belarusian）
* cs - 捷克语（Czech）
* cy - 威尔士语（Welsh）
* da - 丹麦语（Danish）
* de-1901 - 德语（German,1901orthography）
* de-ch-1901 - 瑞士德语（SwissGerman,1901orthography）
* el-monoton - 现代希腊语（ModernGreek,monotonic）
* el-polyton - 现代希腊语（ModernGreek,polytonic）
* en-gb - 英式英语（BritishEnglish）
* en-us - 美式英语（AmericanEnglish）
* es - 西班牙语（Spanish）
* et - 爱沙尼亚语（Estonian）
* fr - 法语（French）
* ga - 爱尔兰语（Irish）
* gl - 加利西亚语（Galician）
* hr - 克罗地亚语（Croatian）
* hu - 匈牙利语（Hungarian）
* id - 印度尼西亚语（Indonesian）
* is - 冰岛语（Icelandic）
* it - 意大利语（Italian）
* ka - 格鲁吉亚语（Georgian）
* lt - 立陶宛语（Lithuanian）
* lv - 拉脱维亚语（Latvian）
* mk - 马其顿语（Macedonian）
* mn-cyrl - 蒙古语（Mongolian,Cyrillicscript）
* nl - 荷兰语（Dutch）
* pt - 葡萄牙语（Portuguese）
* ru - 俄语（Russian）
* sh-cyrl - 塞尔维亚-克罗地亚语（Serbo-Croatian,Cyrillicscript）
* sh-latn - 塞尔维亚-克罗地亚语（Serbo-Croatian,Latinscript）
* sk - 斯洛伐克语（Slovak）
* sl - 斯洛文尼亚语（Slovenian）
* sr-cyrl - 塞尔维亚语（Serbian,Cyrillicscript）
* sv - 瑞典语（Swedish）
* th - 泰语（Thai）
* tk - 土库曼语（Turkmen）
* tr - 土耳其语（Turkish）
* uk - 乌克兰语（Ukrainian）
* zh-latn-pinyin - 汉语拼音（Chinese,Pinyin）

## 目录结构

```
third_party_tex-hyphen
├── collaboration
│   ├── original
│   ├── repository
│   └── source
├── data/language-codes
├── docs
│   └── languages
├── encoding
│   └── data
├── hyph-utf8
│   ├── doc
│   ├── source
│   └── tex
├── misc
├── ohos
│   ├── src
│   └── hpb-binary
├── old
├── source
├── tests
├── TL
├── tools
└── webpage

collaboration/       tex-hyphen官网依赖的js脚本、xml配置文件
ohos/                OpenHarmony编译文件和hpb二进制文件
data/                语种库
docs/                项目的文档资料
encoding/            包含编码相关的文件，用于处理不同字符集的编码问题。
hyph-utf8/           TeX 的断字模式包，提供了以 UTF-8 编码的断字模式
misc/                en-gb语种断词文件案例
old/                 包含一些旧的断词模式文件，这些文件可能已经被更新或替换。
source/              包含源代码文件，用于生成和处理断词模式。
TL/                  tlpsrc资源文件，tlpsrc文件是TeX Live系统中的一个包源文件，用于描述TeX Live包的元数据
tools/               包含一些工具脚本，用于辅助处理断词模式文件。
webpage/             tex-hyphen官网主页，提供了关于 hyph-utf8 包的详细信息和资源
```

## 为 OpenHarmony 带来的价值

**1.提高排版质量：** 通过引入 tex-hyphen，OpenHarmony 可以实现更为精准的断字处理，提高文档的可读性和美观度。  
**2.提升小屏设备体验：** 在小屏设备中使用断词模式，能够在相同区域内显示更多内容，提升阅读体验。

## OpenHarmony中如何使用tex-hyphen

### 1、编译hpb二进制
#### 编译步骤
打开终端（或命令提示符），导航到包含 [hyphen_pattern_processor.cpp](ohos%2Fsrc%2Fhyphen-build%2Fhyphen_pattern_processor.cpp) 文件的目录，并运行以下命令来编译代码：

```
cd ohos/src/hyphen-build/
g++ -g -Wall hyphen_pattern_processor.cpp -o transform
```
上述命令说明：
- g++: 调用 GCC 编译器。  
- -g: 添加调试信息。  
- -Wall: 启用所有警告。  
- hyphen_pattern_processor.cpp: 源代码文件。  
- -o transform: 指定输出的可执行文件名为 transform。

#### 运行步骤
编译完成后，可以使用以下命令来运行生成的可执行文件，并处理指定的 .tex 文件：
```
./transform hyph-en-us.tex ./out/
```
上述命令说明：
- ./transform: 运行生成的 transform 可执行文件。  
- hyph-en-us.tex: 输入文件（待处理的 .tex 文件）。  
- ./out/: 输出目录（处理后的文件将存储在此目录中）。  

运行成功后，处理后的.hpb二进制文件将存储在 ./out/ 目录中。

#### 批量编译
- 依赖：  
```
jq：json文件解析工具
```

- 通过json配置文件[build-tex.json](ohos%2Fbuild%2Fbuild-tex.json)，配置需要编译的文件。
```
[
    {
        "filename": "example1.tex"
    },
    {
        "filename": "example2.tex"
    }
]
```
**filename** ：指定需要编译的tex文件名，文件须在 [tex](hyph-utf8%2Ftex%2Fgeneric%2Fhyph-utf8%2Fpatterns%2Ftex) 目录下  

build-tex.json文件中定义了全量支持语种，脚本会默认全量编译。开发者可通过修改build-tex.json来控制新增或者删除语种。  
例如：  
需要移除example2语种，修改结果如下：
```
[
    {
        "filename": "example1.tex"
    }
]
```
需要新增example3语种，修改结果如下：
```
[
    {
        "filename": "example1.tex"
    },
    {
        "filename": "example2.tex"
    },
    {
        "filename": "example3.tex"
    }
]
```

- 打开终端（或命令提示符），导航到包含 [build.sh](ohos%2Fbuild%2Fbuild.sh) 文件的目录，并运行以下命令来编译代码  
```
chmod +x build.sh
./build.sh
```
编译成功后，编译产物将会放置在./out_hpb目录下

### 2、通过hpb解析单词断词位置
#### 编译步骤
打开终端（或命令提示符），导航到包含 [hyphen_pattern_reader.cpp](ohos%2Fsrc%2Fhyphen-build%2Fhyphen_pattern_reader.cpp) 文件的目录，并运行以下命令来编译代码：

```
cd ohos/src/hyphen-build/
g++ -g -Wall hyphen_pattern_reader.cpp -o reader
```
上述命令说明：
- g++: 调用 GCC 编译器。
- -g: 添加调试信息。
- -Wall: 启用所有警告。
- hyphen_pattern_reader.cpp: 源代码文件。
- -o reader: 指定输出的可执行文件名为 reader。

#### 运行步骤
编译完成后，可以使用以下命令来解析对应语种的单词：
```
./reader hyph-en-us.hpb helloworld
```
上述命令说明：
- ./reader: 运行生成的 reader 可执行文件。
- hyph-en-us.hpb: 输入文件（待解析的二进制文件）。
- helloworld: 待解析的单词。

运行成功后，日志中将会输出本次解析的单词断词信息。

### 3、自动化验证
通过[generate_report.py](ohos%2Ftest%2Fgenerate_report.py) Python脚本读取[report_config.json](ohos%2Ftest%2Freport_config.json)配置文件，可实现批量校验生成的二进制文件是否有效
#### 准备
- Python 3.x
- transform和reader可执行文件，并将可执行文件放在脚本同一级目录
- report_config.json配置文件
#### 使用方法
1. 准备配置文件  
首先，创建一个JSON格式的配置文件report_config.json，包含以下内容：
```
   {
       "file_path": "path/to/tex/files",
       "tex_files": [
           {
           "filename": "example.tex",
           "words": ["word1", "word2", "word3", "word4", "word5", "word6", "word7", "word8", "word9", "word10"]
           },
       ...
       ]
   }
```
- file_path：TeX文件所在的目录路径。
- tex_files：包含多个TeX文件及其对应的单词列表。
2. 运行脚本  
在终端中运行以下命令：
```
python generate_report.py report_config.json
```
3. 日志文件  
脚本会在 report 目录下生成一个带有时间戳的子目录，包含以下日志文件：
```
match.log：记录匹配成功的结果。
unmatch.log：记录匹配失败的结果。
```