# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'gui/ui/LibraryChoose.ui'
#
# Created by: PyQt5 UI code generator 5.15.7
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_LibraryChoose(object):
    def setupUi(self, LibraryChoose):
        LibraryChoose.setObjectName("LibraryChoose")
        LibraryChoose.setWindowModality(QtCore.Qt.WindowModal)
        LibraryChoose.resize(420, 110)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(LibraryChoose.sizePolicy().hasHeightForWidth())
        LibraryChoose.setSizePolicy(sizePolicy)
        LibraryChoose.setMinimumSize(QtCore.QSize(420, 110))
        LibraryChoose.setMaximumSize(QtCore.QSize(420, 110))
        self.verticalLayoutWidget = QtWidgets.QWidget(LibraryChoose)
        self.verticalLayoutWidget.setGeometry(QtCore.QRect(0, 0, 421, 111))
        self.verticalLayoutWidget.setObjectName("verticalLayoutWidget")
        self.gridLayout = QtWidgets.QGridLayout(self.verticalLayoutWidget)
        self.gridLayout.setSizeConstraint(QtWidgets.QLayout.SetMinAndMaxSize)
        self.gridLayout.setContentsMargins(0, 10, 0, 0)
        self.gridLayout.setObjectName("gridLayout")
        self.lineEdit = QtWidgets.QLineEdit(self.verticalLayoutWidget)
        self.lineEdit.setText("")
        self.lineEdit.setObjectName("lineEdit")
        self.gridLayout.addWidget(self.lineEdit, 0, 0, 1, 1)
        self.pushButton = QtWidgets.QPushButton(self.verticalLayoutWidget)
        self.pushButton.setObjectName("pushButton")
        self.gridLayout.addWidget(self.pushButton, 1, 0, 1, 1)
        self.buttonBox = QtWidgets.QDialogButtonBox(self.verticalLayoutWidget)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName("buttonBox")
        self.gridLayout.addWidget(self.buttonBox, 2, 0, 1, 1)

        self.retranslateUi(LibraryChoose)
        self.buttonBox.accepted.connect(LibraryChoose.accept) # type: ignore
        self.buttonBox.rejected.connect(LibraryChoose.reject) # type: ignore
        QtCore.QMetaObject.connectSlotsByName(LibraryChoose)

    def retranslateUi(self, LibraryChoose):
        _translate = QtCore.QCoreApplication.translate
        LibraryChoose.setWindowTitle(_translate("LibraryChoose", "Выбор библиотеки"))
        self.lineEdit.setPlaceholderText(_translate("LibraryChoose", "Выберите путь к библиотеке"))
        self.pushButton.setText(_translate("LibraryChoose", "Выбрать"))
