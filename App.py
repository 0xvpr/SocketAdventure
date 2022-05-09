#!/usr/bin/env python.exe

import sys
import socket
import ctypes

from PyQt5.QtCore import (
    Qt,
    QThread
)

from typing import (
    List,
    NamedTuple,
)

from PyQt5.QtWidgets import (
    QApplication,
    QLabel,
    QWidget
)

# ------------ Global Variables -------------------#

HOST = "127.0.0.1"
PORT = 8888

# ------------- Windows Imports -------------------#

NULL = ctypes.c_void_p(0)
FindWindowA = ctypes.windll.user32.FindWindowA
GetWindowRect = ctypes.windll.user32.GetWindowRect

class RECT(ctypes.Structure):
    _fields_ = [
        ("left",   ctypes.c_long),
        ("top",    ctypes.c_long),
        ("right",  ctypes.c_long),
        ("bottom", ctypes.c_long)
    ]

# ------------- Application Classes ---------------#

class ServerAddr(NamedTuple):
    host: str
    port: int

class Listener(QThread):
    def __init__(self, server_addr: ServerAddr, app: QApplication, parent=None) -> None:
        super().__init__(parent)
        self.server_addr = server_addr
        self.app = app
        self.exiting = False
    
    def __del__(self) -> None:
        self.exiting = True
        self.wait()

    def run(self) -> None:
        print("[+] Connecting to client...")

        attempts = 0
        connected = False
        while not connected:
            try:
                self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.s.connect(self.server_addr)
                connected = True
            except Exception as ex:
                attempts += 1
                print(f"[-] Exception: {ex}\nretrying ({attempts})...")
        print("[+] Connected.")

        while not self.exiting:
            data = self.s.recv(4)
            if data == b"EOF" or data == b"":
                sys.exit(0)
            else:
                operation = int.from_bytes(data, byteorder="little")

                if operation & (1 << 0):
                    self.app.infinite_mana = True
                    self.app.mana_label.setText("Mana: Active")
                else:
                    self.app.infinite_mana = False
                    self.app.mana_label.setText("Mana: Disabled")
                if operation & (1 << 1):
                    pass
                else:
                    pass
                if operation & (1 << 2):
                    pass
                else:
                    pass
                if operation & (1 << 3):
                    pass
                else:
                    pass
                if operation & (1 << 4):
                    pass
                else:
                    pass
                if operation & (1 << 5):
                    pass
                else:
                    pass
                if operation & (1 << 6):
                    pass
                else:
                    pass
                if operation & (1 << 7):
                    pass
                else:
                    pass
                if operation & (1 << 8):
                    pass
                else:
                    pass

class Application(QApplication):
    def __init__(self, argv: List[str], rect: RECT) -> None:
        super().__init__(argv)
        self.server_addr   = ServerAddr(host=HOST, port=PORT)
        self.infinite_mana = False
        self.rect          = rect

        self._setup()
        self.listen_thread = Listener(self.server_addr, self)
        self.listen_thread.start()

        sys.exit(super().exec_())

    def _setup(self) -> None:
        imul = lambda x: int(x * 1.25)

        self.main_window = QWidget()
        self.main_window.setWindowTitle(f"VPR - ({sys.platform})")
        self.main_window.setGeometry(
            imul(self.rect.left),
            imul(self.rect.top),
            imul(self.rect.right - self.rect.left),
            imul(self.rect.bottom - self.rect.top)
        )

        self._add_labels(self.main_window)

        self.main_window.setWindowFlags( Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint)
        self.main_window.setAttribute(Qt.WA_TranslucentBackground)
        self.main_window.show()

    def _add_labels(self, main_window: QWidget) -> None:
        self.mana_label = QLabel(f"Mana: {'Active' if self.infinite_mana else 'Disabled'}", main_window)
        self.mana_label.setGeometry(80, 35, 150, 25)

if __name__ == "__main__":
    hWnd = None
    print("[+] Searching for window...")
    while not hWnd:
        hWnd = FindWindowA(NULL, b"PwnAdventure3 (32-bit, PCD3D_SM5)")
    print("[+] Found.\n")

    rect = RECT()
    GetWindowRect(hWnd, ctypes.byref(rect))

    print(f"left:\t{rect.left}\nright:\t{rect.right}\ntop:\t{rect.top}\nbottom:\t{rect.bottom}\n")

    Application(sys.argv, rect)
