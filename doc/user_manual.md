# Topuino 用户操作手册

## 使用前准备

1. Topuino 依赖 2.4G Wi-Fi 连接互联网，并且不支持 5G Wi-Fi，所以你需要有一个可以连接互联网的 2.4G Wi-Fi 接入点，并且 SSID 不可以包含特殊字符（包括中文字符），且必须有密码；
2. Topuino 通过 HTTP 协议与 iot.vvzero.com 通讯以获取实时监控数据，所以请确保网络链路中不存在拦截、缓存等异常行为；
3. Topuino 使用 USB TypeC 接口供电，供电电压为 5V，市面上绝大多数手机充电器、电脑 USB 输出均可；
4. 客户端需要运行一个 python 脚本以向 iot.vvzero.com 上传监控数据，所以请安装 python3；
5. 客户端、硬件端通过 Topuino 硬件背面贴着的 SN 号进行绑定，请留意。

## 待监控的服务器/电脑配置

1. 安装 python 依赖包 requests 和 psutil，注意 psutil 在某些机器上可能因为缺少某些库而 build 失败（比如 armbian），请用搜索引擎搜索对应的报错信息，一般很容易解决；
2. 下载 [客户端 python 脚本文件](https://git.vvzero.com/villivateur/Topuino_Client/raw/branch/master/main.py)；
3. 修改 main.py，将硬件背面的 SN 填入 main.py 的指定位置；同时请填入需要监控的两个磁盘挂载点；
4. 直接使用 python3 运行 main.py，将会每隔 1s 向 iot.vvzero.com 报告当前监控数据；
5. 如果需要配置自动启动和 daemon，建议参考 [这个 systemd 服务文件](https://git.vvzero.com/villivateur/Topuino_Client/raw/branch/master/topuino_client.service)

## Topuino 配置

1. Topuino 初次上电后，右上角的红色状态灯应当以 0.5 Hz 闪烁（一亮一灭表示一个周期），表示正在等待配置；
2. 以安卓手机配置为例，为避免某些手机的自动网络优化，请暂时关闭数据流量；
3. 请暂时关闭手机上的任何代理软件；
4. 打开 Wi-Fi，并连接 SSID 为 `Topuino` 的接入点，密码是 `vvzero.com`。 如果系统提示该网络无法访问互联网，请忽略并继续保持连接；
5. 打开一个正常的浏览器，访问 192.168.1.1；
6. 在网页中填入 Topuino 可连接的、可访问互联网的 2.4G Wi-Fi 接入点的 SSID 和密码；
7. 点击提交后，页面会返回一个 OK（某些情况下可能会出现 Connection Reset，请忽略）；
8. 此时 Topuino 的状态灯应当以 2Hz 闪烁，等待几秒至十几秒后，状态灯熄灭，面板开始正常显示。

## 常见问题分析

- Q：我遇到问题了怎么办？A：请再认真读一遍该手册，如果无法解决，请继续往下看。
- Q：上电后闪灯状态不对怎么办？A：请先短按 RESET 键并观察，如果不行，请长按 FUNC 键 6 秒以上，再次观察，如果还是不行，请联系开发者。
- Q：我在配置 Topuino 时输错了 SSID、密码怎么办？A：请长按 FUNC 键 6 秒以上，重新配置。
