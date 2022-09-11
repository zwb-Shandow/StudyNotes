# Desktop 快捷方式创建

新建 `*.desktop` 文件并配置。

```shell
sudo vim /usr/share/applications/Typora.desktop
```



快捷方式图标可自行下载，或采用 Typora 软件包里的 icon 图片。

```

[Desktop Entry]
Encoding=UTF-8
Name=Typora
Exec=/opt/Typora-linux-x64/Typora
Terminal=false
Type=Application
Icon=/opt/TYpora-linux-x64/resources/asserts/icon/icon_256x256.png
StartupNotify=true
Categories=Development
```

