import os

from PyQt6.QtWidgets import QMainWindow, QMessageBox, QLabel
from PyQt6.QtCore import pyqtSlot
from PyQt6.QtGui import QPixmap

from GUI.Ui_MainWindow import Ui_MainWindow
from screens.LibraryChooseDialog import LibraryChooseDialog
from config.GalleryConfig import GalleryConfig


class MainWindow(QMainWindow, Ui_MainWindow):
    """Main application gallery window"""

    DEFAULT_WINDOW_TITLE: str = 'Empty gallery'

    directory_path: str | None = None

    thumbnail_widgets: list = list()

    def __init__(self, parent=None) -> None:
        """Setup UI objects and window settings"""
        super().__init__(parent)

        self.setupUi(self)
        self.setWindowTitle(self.DEFAULT_WINDOW_TITLE)

    def show(self) -> None:
        """Override default show method to check for saved gallery path"""
        super().show()

        if not self.__is_saved_dir_path():
            dialog_result = self.__open_ask_gallery_path_dialog()
            if dialog_result['accepted']:
                self.__open_path(dialog_result['path'])
        else:
            self.__open_path(self.__get_saved_dir_path())

    @pyqtSlot(name='on_CloseAction_triggered')
    def __closeAction_triggered(self) -> None:
        """Closing opened directory and show an empty gallery"""
        self.__set_directory_path(None)

    @pyqtSlot(name='on_RefreshAction_triggered')
    def __refreshAction_trigerred(self) -> None:
        """Refreshing list of images according to directory"""
        self.__set_directory_path(self.directory_path)
        self.__create_thumbnails()

    def __is_saved_dir_path(self) -> bool:
        """Returns true if the path to the gallery directory was saved at the last launch"""
        if os.path.isfile(f'{os.getcwd()}/{GalleryConfig.SAVE_GALLERY_PATH}') and len(self.__get_saved_dir_path()) > 0:
            return True

        return False

    @staticmethod
    def __get_saved_dir_path() -> str:
        """Returns first line from SAVE_GALLERY_PATH file"""
        with open(f'{os.getcwd()}/{GalleryConfig.SAVE_GALLERY_PATH}', 'r') as file:
            return file.readline()

    @pyqtSlot(name='on_OpenAction_triggered')
    def __openAction_triggered(self):
        dialog_result = self.__open_ask_gallery_path_dialog()
        if dialog_result['accepted']:
            self.__open_path(dialog_result['path'])

    def __open_ask_gallery_path_dialog(self) -> dict:
        """Ask user to select a gallery path"""
        dialog = LibraryChooseDialog(self)
        dialog.exec()

        return {
            'accepted': dialog.result() == LibraryChooseDialog.DialogCode.Accepted,
            'path': dialog.directory_path
        }

    def __set_directory_path(self, directory_path) -> None:
        """Set directory path and window title according to it"""
        for widget in self.thumbnail_widgets:
            self.ThumbnailAreaLayout.removeWidget(widget)

        self.thumbnail_widgets = list()
        self.directory_path = directory_path
        self.setWindowTitle(self.DEFAULT_WINDOW_TITLE if directory_path is None else directory_path)

        with open(f'{os.getcwd()}/{GalleryConfig.SAVE_GALLERY_PATH}', 'w+') as file:
            file.write('' if directory_path is None else directory_path)

    def __chosen_dir_incorrect(self, directory_path):
        message_box = QMessageBox(self)
        message_box.setWindowTitle('Error')
        message_box.setText(
            f'The gallery folder you specified does not look like a real existing folder\n\n{directory_path}')
        message_box.setIcon(QMessageBox.Icon.Critical)
        message_box.exec()

    def __open_path(self, directory_path: str) -> None:
        """Open provided gallery path"""
        if not os.path.isdir(directory_path):
            self.__chosen_dir_incorrect(directory_path)
            return

        self.__set_directory_path(directory_path)
        self.__create_thumbnails()

    def __create_thumbnails(self):
        with os.scandir(self.directory_path) as _:
            for entry in _:
                if entry.is_file and entry.name.endswith(GalleryConfig.IMAGES_EXT):
                    label = QLabel('', parent=self)
                    label.setPixmap(QPixmap(entry.path).scaledToWidth(int(self.ThumbnailArea.width() / 3 - 12)))
                    self.thumbnail_widgets.append(label)
                    self.ThumbnailAreaLayout.addWidget(label)
            if len(self.thumbnail_widgets) % 3 != 0:
                for i in range(0, 3 - len(self.thumbnail_widgets) % 3):
                    __ = QLabel('', parent=self)
                    self.ThumbnailAreaLayout.addWidget(__)
                    self.ThumbnailAreaLayout.removeWidget(__)

