# Socket Adventure (PwnAdventure3 Windows)
### Timeline
- Project Started: May 9, 2022

## Purpose
The purpose of this snippet was to create an application that  
utilizes TCP sockets to provide information to an external  
application that is responsible for the UI.

## What I Learned
[ TODO ]

## Install (WSL2 Version 2)
```bash
git clone https://github.com/0xvpr/SocketAdventure && cd SocketAdventure
make && python -m pip install -r requirements.txt
```

## Run
WSL2
```bash
cd SocketAdventure
chmod +x App.py inject.sh
./inject.py # Requires vpr-pidjeon to be installed
```

Powershell
```powershell
cd SocketAdventure
# Inject with your favorite injector then run App.py for UI
python App.py
```
