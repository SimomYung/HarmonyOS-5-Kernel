# tex-hyphen
## Introduction
tex-hyphen is a hyphenation pattern library for the TeX system. It can correctly hyphenate words in multiple languages to improve typesetting quality.

Source: tex-hyphen  
URL: https://github.com/hyphenation/tex-hyphen  
Version: CTAN-2021.03.21  
License: Various combinations 

## Background
In multilingual document processing and typesetting, correct hyphenation is crucial. tex-hyphen provides a comprehensive set of hyphenation patterns that support multiple languages, ensuring high-quality typesetting. Introducing tex-hyphen into OpenHarmony can significantly enhance the typesetting quality of multilingual documents.

## Language Classification
The tex directory contains multiple hyphenation patterns from TeX hyphenation patterns, each using different open-source licenses. The classification is as follows:
* MIT License
* GPL, GPL 2
* LGPL 1, LGPL 2.1
* LPPL 1, LPPL 1.2, LPPL 1.3
* MPL 1.1
* BSD 3

Languages used in OHOS,the following languages all use a user-friendly open-source license.:
* be - Belarusian
* cs - Czech
* cy - Welsh
* da - Danish
* de-1901 - German (1901 orthography)
* de-ch-1901 - Swiss German (1901 orthography)
* el-monoton - Modern Greek (monotonic)
* el-polyton - Modern Greek (polytonic)
* en-gb - British English
* en-us - American English
* es - Spanish
* et - Estonian
* fr - French
* ga - Irish
* gl - Galician
* hr - Croatian
* hu - Hungarian
* id - Indonesian
* is - Icelandic
* it - Italian
* ka - Georgian
* lt - Lithuanian
* lv - Latvian
* mk - Macedonian
* mn-cyrl - Mongolian (Cyrillic script)
* nl - Dutch
* pt - Portuguese
* ru - Russian
* sh-cyrl - Serbo-Croatian (Cyrillic script)
* sh-latn - Serbo-Croatian (Latin script)
* sk - Slovak
* sl - Slovenian
* sr-cyrl - Serbian (Cyrillic script)
* sv - Swedish
* th - Thai
* tk - Turkmen
* tr - Turkish
* uk - Ukrainian
* zh-latn-pinyin - Chinese (Pinyin)

## Directory Structure
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
```
collaboration/       JavaScript dependencies and XML configuration files required by the tex-hyphen official website  
ohos/                OpenHarmony compilation files and hpb binary files  
data/                Language library  
docs/                Documentation related to hyphenation  
encoding/            Contains files related to character set encodings, handling different character sets.  
hyph-utf8/           Hyphenation pattern package for TeX, providing hyphenation patterns encoded in UTF-8  
misc/                An example of a hyphenation file for the en-gb language.  
old/                 Contains older hyphenation pattern files that may have been updated or replaced.   
source/              Contains source code files used to generate and process hyphenation patterns.  
TL/                  tlpsrc resource files, which are package source files in the TeX Live system, used to describe metadata of TeX Live packages  
tools/               Contains utility scripts to assist in processing hyphenation pattern files.  
webpage/             tex-hyphen official homepage, providing detailed information and resources about the hyph-utf8 package  


## Value Brought to OpenHarmony
**1. Improved Typesetting Quality:** By introducing tex-hyphen, OpenHarmony can achieve more accurate hyphenation, improving the readability and aesthetics of documents.  
**2. Enhanced Small Screen Experience:** Using hyphenation patterns on small screen devices can display more content in the same area, enhancing the reading experience.

## How to Use tex-hyphen in OpenHarmony
### 1. Compile the HPB Binary
#### Compilation Steps
Open the terminal (or command prompt), navigate to the directory containing the [hyphen_pattern_processor.cpp](ohos%2Fsrc%2Fhyphen-build%2Fhyphen_pattern_processor.cpp) file, and run the following command to compile the code:

```
cd ohos/src/hyphen-build/
g++ -g -Wall hyphen_pattern_processor.cpp -o transform
```

Explanation of the command:
- g++: Invoke the GCC compiler.
- -g: Add debugging information.
- -Wall: Enable all warnings.
- hyphen_pattern_processor.cpp: Source code file.
- -o transform: Specify the output executable file name as transform.

#### Execution Steps
After compilation, you can run the generated executable file and process the specified .tex file using the following command:

```
./transform hyph-en-us.tex ./out/
```

Explanation of the command:
- ./transform: Run the generated transform executable file.
- hyph-en-us.tex: Input file (the .tex file to be processed).
- ./out/: Output directory (the processed files will be stored in this directory).

After successful execution, the processed files will be stored in the ./out/ directory.

#### Batch Compilation
- Dependencies:
```
jq:JSON file parsing tool
```
- Configure the files to be compiled using the JSON configuration file [build-tex.json](ohos%2Fbuild%2Fbuild-tex.json):
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
filename: Specifies the name of the TeX file to be compiled. The file must be located in the [tex](hyph-utf8%2Ftex%2Fgeneric%2Fhyph-utf8%2Fpatterns%2Ftex) directory.  

The build-tex.json file defines all supported languages, and the script will compile all of them by default. Developers can control the addition or removal of languages by modifying build-tex.json.   
For example:  
To remove the example2 language, modify the file as follows:
```
[
    {
        "filename": "example1.tex"
    }
]
```
To add the example3 language, modify the file as follows:
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

- Open a terminal (or command prompt), navigate to the directory containing the [build.sh](ohos%2Fbuild%2Fbuild.sh) file, and run the following commands to compile the code:
```
chmod +x build.sh
./build.sh
```
After successful compilation, the compiled output will be placed in the ./out_hpb directory.
### 2. Parse Word Hyphenation Positions Using HPB
#### compilation Steps
Open a terminal (or command prompt), navigate to the directory containing the [hyphen_pattern_reader.cpp](ohos%2Fsrc%2Fhyphen-build%2Fhyphen_pattern_reader.cpp) file, and run the following command to compile the code:

```
cd ohos/src/hyphen-build/
g++ -g -Wall hyphen_pattern_reader.cpp -o reader
```
Explanation of the command:
- g++: Calls the GCC compiler.
- -g: Adds debugging information.
- -Wall: Enables all warnings.
- hyphen_pattern_reader.cpp: The source code file.
- -o reader: Specifies the output executable file name as reader.

#### Running Steps
After compilation, you can parse the hyphenation positions of words in the specified language using the following command:

```
./reader hyph-en-us.hpb helloworld
```
Explanation of the command:
- ./reader: Runs the generated reader executable.
- hyph-en-us.hpb: The input file (the binary file to be parsed).
- helloworld: The word to be parsed.

After successful execution, the log will output the hyphenation information of the parsed word.

### 3. Batch Verification 
You can use the [generate_report.py](ohos%2Ftest%2Fgenerate_report.py) Python script to read the [report_config.json](ohos%2Ftest%2Freport_config.json) configuration file and perform batch verification to check the validity of the generated binary files.  
#### Preparation
- Python 3.x
- transform and reader executables, placed in the same directory as the script.
- report_config.json configuration file

#### Usage
1. Prepare the Configuration File First, create a JSON configuration file named report_config.json with the following content:
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
2. Run the Script Run the following command in the terminal:
```
python generate_report.py report_config.json
```
3. Log Files The script will generate a timestamped subdirectory under the report directory, containing the following log files:
```
match.log: Records successful matches.
unmatch.log: Records unsuccessful matches.
```