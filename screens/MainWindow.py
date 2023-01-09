import math
import os
from random import shuffle

from PyQt6.QtWidgets import QMainWindow, QMessageBox, QProgressBar
from PyQt6.QtCore import pyqtSlot, pyqtSignal, Qt
from PyQt6.QtGui import QPixmap, QIcon

from functions.intervalFunc import setInterval
from GUI.Ui_MainWindow import Ui_MainWindow
from widgets.Thumbnail import Thumbnail
from screens.LibraryChooseDialog import LibraryChooseDialog
from config.GalleryConfig import GalleryConfig


class MainWindow(QMainWindow, Ui_MainWindow):
    """Main application gallery window"""

    DEFAULT_WINDOW_TITLE: str = 'Empty gallery'

    columns: int = 3
    max_columns: int = 8
    min_columns: int = 1

    directory_path: str | None = None

    resized = pyqtSignal()

    thumbnail_lists: dict[str, list[Thumbnail]]

    current_list: str

    preview_thumbnail: Thumbnail | None = None
    privew_pixmap: QPixmap | None = None

    stopSlideshow = None
    currentSlide = None

    spacePressing: bool = False

    def __init__(self, parent=None) -> None:
        """Setup UI objects and window settings"""
        super().__init__(parent)
        self.setupUi(self)
        self.resized.connect(self.__resized)
        self.SplitterBody.splitterMoved.connect(self.__resized)
        self.resetThumbnailLists()
        self.setWindowTitle(self.DEFAULT_WINDOW_TITLE)
        self.setWindowIcon(
            QIcon(':/theme/icons/red-panda.png'))

    def resetThumbnailLists(self):
        self.thumbnail_lists = {
            'original': list(),
            'sortedByDominantColour': list(),
            'shuffled': list()
        }

    def resizeEvent(self, event):
        self.resized.emit()
        return super().resizeEvent(event)

    def post_initialize(self) -> None:
        if not self.__is_saved_dir_path():
            dir_path = ""
        else:
            dir_path = self.__get_saved_dir_path()

        dialog_result = self.__open_ask_gallery_path_dialog(dir_path)
        if dialog_result['accepted']:
            self.__open_path(dialog_result['path'])

    def __resized(self):
        self.__show_preview()
        self.__resize_thumbnails()

    def __resize_thumbnails(self):
        for thumbnail in self.thumbnail_lists['original']:
            thumbnail.setScaledIcon(self.__get_thumbnail_width())

    def __set_current_thumbnail(self, thumbnail: Thumbnail | None):
        if thumbnail is None:
            self.preview_thumbnail = None
            self.privew_pixmap = self.preview_thumbnail = None
            return

        self.preview_thumbnail = thumbnail
        self.privew_pixmap = QPixmap(thumbnail.origin_path)
        self.__show_preview()

    def __stopSlideshow(self):
        if self.stopSlideshow is not None:
            self.stopSlideshow.set()
            self.stopSlideshow = None

    def keyPressEvent(self, event):
        if event.isAutoRepeat():
            return
        self.spacePressing = event.key() == 32
        print(self.spacePressing)

    def keyReleaseEvent(self, event):
        if event.isAutoRepeat():
            return
        self.spacePressing = False if event.key() == 32 else self.spacePressing
        print(self.spacePressing)

    @pyqtSlot(name='on_StopSlideshowAction_triggered')
    def __StopSlideshowAction_triggered(self):
        self.__stopSlideshow()

    @pyqtSlot(name='on_SlideshowAction_triggered')
    def __SlideshowAction_triggered(self) -> None:
        currentList = self.thumbnail_lists[self.current_list]
        self.currentSlide = 0
        if self.preview_thumbnail:
            self.currentSlide = currentList.index(self.preview_thumbnail)

        def setSlide(slideIndex: int):
            self.__set_current_thumbnail(currentList[slideIndex])

        def next():
            if self.currentSlide is None or self.spacePressing:
                return

            self.currentSlide += 1
            if self.currentSlide >= len(currentList):
                self.currentSlide = 0

            setSlide(self.currentSlide)

        setSlide(self.currentSlide)
        self.stopSlideshow = setInterval(next, 2.5)

    @pyqtSlot(name='on_MoreColumnsAction_triggered')
    def __MoreColumnsAction_triggered(self) -> None:
        self.__setColumnDelta(1)

    @pyqtSlot(name='on_LessColumnsAction_triggered')
    def __LessColumnsAction_triggered(self) -> None:
        self.__setColumnDelta(-1)

    def __setColumnDelta(self, delta: int):
        if self.columns + delta < self.min_columns or self.columns + delta > self.max_columns:
            return

        self.columns += delta
        self.ThumbnailAreaLayout
        self.__clear_thumbnail_area()
        self.__render_thumbnails()
        self.__resize_thumbnails()

    @pyqtSlot(name='on_ShuffleAction_triggered')
    def __ShuffleAction_triggered(self):
        self.thumbnail_lists['shuffled'] = \
            self.thumbnail_lists['original'].copy()
        shuffle(self.thumbnail_lists['shuffled'])
        self.__render_thumbnails('shuffled')

    @pyqtSlot(name='on_CloseAction_triggered')
    def __closeAction_triggered(self) -> None:
        """Closing opened directory and show an empty gallery"""
        self.__stopSlideshow()
        self.__set_directory_path(None)
        self.__show_preview()

    @pyqtSlot(name='on_RefreshAction_triggered')
    def __refreshAction_trigerred(self) -> None:
        """Refreshing list of images according to directory"""
        self.__stopSlideshow()
        self.__set_directory_path(self.directory_path)
        self.__show_preview(True)
        self.__create_thumbnails()

    @pyqtSlot(name='on_OpenAction_triggered')
    def __openAction_triggered(self):
        self.__stopSlideshow()
        dialog_result = self.__open_ask_gallery_path_dialog()
        if dialog_result['accepted']:
            self.__set_directory_path(self.directory_path)
            self.__show_preview()
            self.__open_path(dialog_result['path'])

    @pyqtSlot(name='on_ByColorAction_triggered')
    def __byColorAction_triggered(self):
        self.__stopSlideshow()
        progress_bar = QProgressBar()
        progress_bar.setMaximum(len(self.thumbnail_lists['original']))
        progress_bar.setValue(0)
        self.StatusBar.addWidget(progress_bar)

        for thumbnail in self.thumbnail_lists['original']:
            thumbnail.determineDominantColour()
            progress_bar.setValue(progress_bar.value() + 1)

        self.thumbnail_lists['sortedByDominantColour'] = self.thumbnail_lists['original'].copy(
        )

        self.__clear_thumbnail_area()
        self.thumbnail_lists['sortedByDominantColour'].sort(
            key=lambda t: t.dominant_colour)  # type: ignore
        self.__render_thumbnails('sortedByDominantColour')

        self.StatusBar.removeWidget(progress_bar)

    @pyqtSlot(name='on_UnsortedAction_triggered')
    def __unsortedAction_triggered(self):
        self.__stopSlideshow()
        self.__clear_thumbnail_area()
        self.__render_thumbnails('original')

    def __show_preview(self, set_empty: bool = False):
        sender = self.sender()

        if set_empty or not isinstance(sender, Thumbnail):
            self.set_pixmap_info()
            self.PreviewLabel.clear()

        if isinstance(sender, Thumbnail):
            self.__stopSlideshow()

            self.preview_thumbnail = sender
            self.set_pixmap_info(sender)
            self.__set_preview(QPixmap(sender.origin_path))
        else:
            if self.preview_thumbnail is not None and self.privew_pixmap is not None:
                self.set_pixmap_info(self.preview_thumbnail)
                self.__set_preview(self.privew_pixmap)

    def set_pixmap_info(self, thumbnail: Thumbnail | None = None):
        file_name = ''
        resolution = ''

        if thumbnail is not None:
            file_name = os.path.basename(thumbnail.origin_path)
            if len(file_name) > 20:
                file_name = file_name[0:15] + '...' + file_name.split('.')[-1]
            resolution = f'{thumbnail.source_size.width()}x{thumbnail.source_size.height()}'

        self.FileNameLabel.setText(file_name)
        self.ResolutionLabel.setText(resolution)

    def __set_preview(self, pixmap: QPixmap):
        self.privew_pixmap = preview = pixmap
        w = self.PreviewLabel.width()
        h = self.PreviewLabel.height()

        if w < h:
            preview = pixmap.scaledToWidth(w)
        else:
            preview = pixmap.scaledToHeight(h)

        if preview.height() > h:
            preview = pixmap.scaledToHeight(h)
        elif preview.width() > w:
            preview = pixmap.scaledToWidth(w)

        self.PreviewLabel.setPixmap(preview)

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

    def __open_ask_gallery_path_dialog(self, dir_path: str = "") -> dict:
        """Ask user to select a gallery path"""
        dialog = LibraryChooseDialog(dir_path, self)
        dialog.exec()

        return {
            'accepted': dialog.result() == LibraryChooseDialog.DialogCode.Accepted,
            'path': dialog.directory_path
        }

    def __set_directory_path(self, directory_path) -> None:
        """Set directory path and window title according to it"""
        self.__clear_thumbnail_area()
        self.resetThumbnailLists()

        self.directory_path = directory_path
        self.setWindowTitle(
            self.DEFAULT_WINDOW_TITLE if directory_path is None else directory_path)
        self.__set_current_thumbnail(None)

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

    def __get_thumbnail_width(self) -> int:
        col_width = int(self.ThumbnailArea.width() / self.columns - 12)

        return col_width

    def __create_thumbnails(self):
        progress_bar = QProgressBar()
        self.StatusBar.addWidget(progress_bar)
        image_entries = list()

        with os.scandir(self.directory_path) as _:
            for entry in _:
                if entry.is_file and entry.name.lower().endswith(GalleryConfig.IMAGES_EXT):
                    image_entries.append(entry)

        progress_bar.setMaximum(len(image_entries))
        progress_bar.setValue(0)

        for entry in image_entries:
            thumbnail = Thumbnail(
                self, entry.path, self.__get_thumbnail_width())
            thumbnail.clicked.connect(self.__show_preview)
            thumbnail.deleted.connect(self.__delete_thumb)

            self.thumbnail_lists['original'].append(thumbnail)
            progress_bar.setValue(progress_bar.value() + 1)

        self.StatusBar.removeWidget(progress_bar)
        self.__render_thumbnails()

    def __delete_thumb(self, thumbnail):
        self.preview_thumbnail = None
        self.privew_pixmap = None
        self.__show_preview(True)

        if thumbnail not in self.thumbnail_lists[self.current_list]:
            return

        deleted_index = self.thumbnail_lists[self.current_list].index(
            thumbnail)

        for i in self.thumbnail_lists.values():
            if thumbnail in i:
                i.remove(thumbnail)

        self.__remove_widget(thumbnail)
        # self.__clear_thumbnail_area()
        self.__render_thumbnails(
            list_name=self.current_list, start_from=deleted_index)

    def __remove_widget(self, widget):
        self.ThumbnailAreaLayout.removeWidget(widget)
        # widget.deleteLater() # This cause to exception that C\C++ wrapped object of type Thu,bnail has been deleted

    def __clear_thumbnail_area(self):
        for widget in self.thumbnail_lists['original']:
            self.__remove_widget(widget)

    def __render_thumbnails(self, list_name: str = 'original', start_from: int = 0):
        self.current_list = list_name

        thumbnails = self.thumbnail_lists[list_name]
        for i in range(start_from, len(thumbnails)):
            row = math.floor(i / self.columns)
            column = i % self.columns

            self.ThumbnailAreaLayout.addWidget(
                thumbnails[i], row, column, Qt.AlignmentFlag.AlignCenter)
