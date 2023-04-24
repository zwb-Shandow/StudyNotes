# StudyNotes
## Usage
```shell
virtualenv -p /usr/bin/python3.8 venv
source venv/bin/activate
pip install -r requirements.txt
mkdocs serve
```

## Service

mkdocs.service

```shell
[Unit]
Description=Mkdocs

[Service]
Type=simple
User=trunk
ExecStart=/bin/bash -c "cd ~/StudyNotes && source venv/bin/activate && mkdocs serve"
Restart=on-failure
StandardOutput=journal+console

[Install]
WantedBy=multi-user.target
```
