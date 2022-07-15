# 时间同步

> 本文主要从如何对时间同步状态进行监控来进行描述

Linux 下时间同步工具:

- [chrony](#1-chrony)
- [ptpd](#2-ptpd)

## 1. chrony

### 1.1 Usage

```shell
# install
sudo apt install chrony
# 查看时间同步状态
chronyc tracking
```

### 1.2 Monitor

对 `chronyc tracking` 返回值进行解析，主要关注 `System time` 即可。

```
Reference ID    : 4E2E66B4 (electrode.felixc.at)
Stratum         : 4
Ref time (UTC)  : Fri Jul 15 03:17:30 2022
System time     : 0.003299352 seconds slow of NTP time
Last offset     : -0.000069074 seconds
RMS offset      : 0.021351278 seconds
Frequency       : 768.752 ppm fast
Residual freq   : -0.310 ppm
Skew            : 20.415 ppm
Root delay      : 0.154908478 seconds
Root dispersion : 0.026640367 seconds
Update interval : 64.3 seconds
Leap status     : Normal
```

下面以 python 为例，提取 `System time`。

```python
import re
from subprocess import Popen, PIPE

try:
    p = Popen(["chronyc", "tracking"], stdout=PIPE, stdin=PIPE, stderr=PIPE)
    res = p.wait()
    (o, e) = p.communicate()
except OSError as errno:
    if errno == 4:
        return None
    else:
        raise
    if res == 0:
        lines = o.decode().splitlines(False)
        for line in lines:
            key = line.split(":")[0].strip()
            value = line.split(":")[1].strip()
            if key == "System time":
                system_time = float(re.match(r"\d+\.?\d*", value).group())
	else:
        pass
```

## 2. ptpd

### 2.1 Usage

```shell
# install
sudo apt install ptpd
# ptpd master
ptpd -M -C -i eth0
# ptpd slave
ptpd -C -s -i eth0
```

ptpd 默认不记录时间同步信息，若要记录，则可通过命令行或配置文件设置。文件默认生成在 `/var/run/ptpd2.status`.

```shell
ptpd -C -s -i etho --global:log_status=Y
```

### 2.2 Monitor

对 `ptpd2.status` 文件进行解析，主要关注 `Offset from Master` 字段。

```
Host info          :  zzb-pc, PID 37253
Local time         :  Thu Jul 14 10:09:36 CST 2022
Kernel time        :  Thu Jul 14 02:09:36 GMT 2022
Interface          :  ens33
Preset             :  slaveonly
Transport          :  ipv4, multicast
Delay mechanism    :  E2E
Sync mode          :  TWO_STEP
PTP domain         :  0
Port state         :  PTP_SLAVE
Local port ID      :  000c29fffec73815(unknown)/1
Best master ID     :  54e1adfffe78fbea(unknown)/1
Best master IP     :  192.168.5.88
GM priority        :  Priority1 128, Priority2 128, clockClass 13
Time properties    :  ARB timescale, tracbl: time N, freq N, src: INTERNAL_OSCILLATOR(0xa0)
UTC properties     :  UTC valid: N, UTC offset: 0
Offset from Master :  0.000020483 s, mean  0.000016410 s, dev  0.000284063 s
Mean Path Delay    :  0.000450649 s, mean  0.000479278 s, dev  0.000011656 s
Clock status       :  in control
Clock correction   : -12.828 ppm, mean -12.587 ppm, dev  0.661 ppm
Message rates      :  1/s sync, 1/s delay, 1/2s announce
TimingService      :  current PTP0, best PTP0, pref PTP0
TimingServices     :  total 1, avail 1, oper 1, idle 0, in_ctrl 1
Performance        :  Message RX 3/s, TX 0/s
Announce received  :  34
Sync received      :  67
Follow-up received :  68
DelayReq sent      :  61
DelayResp received :  61
State transitions  :  3
PTP Engine resets  :  1
```

下面以 python 为例，提取 `Offset from Master`。

```python
try:
    with open("/var/run/ptpd2.status") as f:
        for line in f:
            key = line.split(":")[0].strip()
            value = line.split(":")[1].strip()
            if key == "Offset from Master":
                offset = float(re.match(r"-*\d+\.?\d*", value).group())
except IOError:
    pass
```

