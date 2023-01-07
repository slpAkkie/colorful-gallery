from send2trash import send2trash

from PyQt6.QtCore import QSize, Qt, pyqtSignal
from PyQt6.QtGui import QPixmap, QIcon, QAction, QCursor
from PyQt6.QtWidgets import QToolButton, QMenu, QWidget
from functions.getDominantColour import getDominantColour


class Thumbnail(QToolButton):

    context_menu: QMenu

    deleted = pyqtSignal(QWidget)

    max_side_size: int = 1024

    origin_path: str
    pixmap: QPixmap
    source_size: QSize
    dominant_colour: int | None = None

    def __init__(self, parent, path, width):
        super().__init__(parent)

        self.pixmap = QPixmap(path)
        self.source_size = self.pixmap.size()
        if self.pixmap.width() > self.pixmap.height():
            if self.pixmap.width() > self.max_side_size:
                self.pixmap = self.pixmap.scaledToWidth(self.max_side_size)
        else:
            if self.pixmap.height() > self.max_side_size:
                self.pixmap = self.pixmap.scaledToHeight(self.max_side_size)
        self.setScaledIcon(width)
        self.origin_path = path

        self._createContextMenu()

    def _createContextMenu(self):
        self.setContextMenuPolicy(Qt.ContextMenuPolicy.CustomContextMenu)

        self.context_menu = QMenu(parent=self, title='Контекстное меню')

        delete_action = QAction(parent=self, text='Удалить файл')
        delete_action.triggered.connect(self._delete)
        self.context_menu.addAction(delete_action)

        self.customContextMenuRequested.connect(self.open_context_menu)

    def open_context_menu(self):
        self.context_menu.exec(QCursor.pos())

    def _delete(self):
        send2trash(self.origin_path)
        self.deleted.emit(self)

    def setScaledIcon(self, width):
        scaled_pixmap = self.pixmap.scaledToWidth(width)
        size = QSize(width, scaled_pixmap.height())
        self.setFixedSize(size)
        self.setIconSize(size)
        self.setIcon(QIcon(scaled_pixmap))

    def determineDominantColour(self):
        if self.dominant_colour is None:
            self.dominant_colour = getDominantColour(self.pixmap)
