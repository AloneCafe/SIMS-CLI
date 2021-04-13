# SIMS-CLI（基于CLI的学生信息管理系统Demo）

**说实话，这个程序可以说设计的没有一点实用价值(～￣▽￣)～，这个程序的初衷是为了应付老同学的大学C语言课题设计（用的原生态的ASCII编码保存数据，无任何数据库结构），如果说这个项目有什么特点的话，除了古怪到爆的操作方式，还有一小部分算法还是值得初学者参考的（笑）。**

**这是一个Very Very Stupid的基于CLI和ASCII数据文件的学生信息管理系统演示程序（SIMS Demo，Students Information Management System Demo）（目前仅支持Windows操作系统）**

---
![sims](https://github.com/AloneCafe/SIMS-CLI/raw/master/screenshots/sims.png)
![sims](https://github.com/AloneCafe/SIMS-CLI/raw/master/screenshots/help.png)
## 获取源代码
### Git版本控制工具：
* **<code>git clone https://github.com/AloneCafe/SIMS-CLI.git</code>**

### 下载zip压缩包：
* **<a href="https://github.com/AloneCafe/SIMS-CLI/archive/master.zip">项目ZIP压缩包</a>**

---

## 构建项目
### Windows操作系统（最好把MinGW中的bin目录添加到系统的PATH环境变量中）
**一、编译源代码**
* 使用MinGW make自动构建：
  **<p><code>mingw32-make -f Makefile.win</code></p>**
* 使用MinGW GCC手动编译构建：
  **<p>GCC编译：<code>gcc -o .\build\sims.exe sims.c</code><br/>**
  **拷贝数据文件模板：<code>copy students.dat.example .\build\students.dat</code></p>**

## 关于数据文件
**所有学生的数据是以一行一行的记录的形式，使用ASCII文本保存在与程序同目录下的students.dat文件中，以简单的明文形式存储**
~~为保护他人隐私，已移除可能涉及个人信息的图片~~
