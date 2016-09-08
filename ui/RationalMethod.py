# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'RationalMethod.ui'
#
# Created: Tue Oct  7 21:35:56 2014
#      by: PyQt4 UI code generator 4.9.6
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_RationalMethod(object):
    def setupUi(self, RationalMethod):
        RationalMethod.setObjectName(_fromUtf8("RationalMethod"))
        RationalMethod.resize(357, 230)
        self.verticalLayout_3 = QtGui.QVBoxLayout(RationalMethod)
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.groupBox_2 = QtGui.QGroupBox(RationalMethod)
        self.groupBox_2.setObjectName(_fromUtf8("groupBox_2"))
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.groupBox_2)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.formLayout_2 = QtGui.QFormLayout()
        self.formLayout_2.setObjectName(_fromUtf8("formLayout_2"))
        self.labelRunoffCoefficient = QtGui.QLabel(self.groupBox_2)
        self.labelRunoffCoefficient.setObjectName(_fromUtf8("labelRunoffCoefficient"))
        self.formLayout_2.setWidget(0, QtGui.QFormLayout.LabelRole, self.labelRunoffCoefficient)
        self.horizontalLayout_4 = QtGui.QHBoxLayout()
        self.horizontalLayout_4.setObjectName(_fromUtf8("horizontalLayout_4"))
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_4.addItem(spacerItem)
        self.horizontalLayout_3 = QtGui.QHBoxLayout()
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.runoffCoefficient = QtGui.QLineEdit(self.groupBox_2)
        self.runoffCoefficient.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.runoffCoefficient.setObjectName(_fromUtf8("runoffCoefficient"))
        self.horizontalLayout_3.addWidget(self.runoffCoefficient)
        self.getCoefficientButton = QtGui.QToolButton(self.groupBox_2)
        self.getCoefficientButton.setObjectName(_fromUtf8("getCoefficientButton"))
        self.horizontalLayout_3.addWidget(self.getCoefficientButton)
        self.horizontalLayout_4.addLayout(self.horizontalLayout_3)
        self.formLayout_2.setLayout(0, QtGui.QFormLayout.FieldRole, self.horizontalLayout_4)
        self.labelRainFallIntensity = QtGui.QLabel(self.groupBox_2)
        self.labelRainFallIntensity.setObjectName(_fromUtf8("labelRainFallIntensity"))
        self.formLayout_2.setWidget(1, QtGui.QFormLayout.LabelRole, self.labelRainFallIntensity)
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        spacerItem1 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_2.addItem(spacerItem1)
        self.rainfallIntensity = QtGui.QLineEdit(self.groupBox_2)
        self.rainfallIntensity.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.rainfallIntensity.setObjectName(_fromUtf8("rainfallIntensity"))
        self.horizontalLayout_2.addWidget(self.rainfallIntensity)
        self.formLayout_2.setLayout(1, QtGui.QFormLayout.FieldRole, self.horizontalLayout_2)
        self.labelArea = QtGui.QLabel(self.groupBox_2)
        self.labelArea.setObjectName(_fromUtf8("labelArea"))
        self.formLayout_2.setWidget(2, QtGui.QFormLayout.LabelRole, self.labelArea)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        spacerItem2 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem2)
        self.watershedArea = QtGui.QLineEdit(self.groupBox_2)
        self.watershedArea.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.watershedArea.setObjectName(_fromUtf8("watershedArea"))
        self.horizontalLayout.addWidget(self.watershedArea)
        self.formLayout_2.setLayout(2, QtGui.QFormLayout.FieldRole, self.horizontalLayout)
        self.verticalLayout_2.addLayout(self.formLayout_2)
        self.verticalLayout_3.addWidget(self.groupBox_2)
        self.groupBox_4 = QtGui.QGroupBox(RationalMethod)
        self.groupBox_4.setObjectName(_fromUtf8("groupBox_4"))
        self.horizontalLayout_5 = QtGui.QHBoxLayout(self.groupBox_4)
        self.horizontalLayout_5.setObjectName(_fromUtf8("horizontalLayout_5"))
        self.formLayout = QtGui.QFormLayout()
        self.formLayout.setObjectName(_fromUtf8("formLayout"))
        self.labelRunoff = QtGui.QLabel(self.groupBox_4)
        self.labelRunoff.setObjectName(_fromUtf8("labelRunoff"))
        self.formLayout.setWidget(0, QtGui.QFormLayout.LabelRole, self.labelRunoff)
        self.horizontalLayout_8 = QtGui.QHBoxLayout()
        self.horizontalLayout_8.setObjectName(_fromUtf8("horizontalLayout_8"))
        spacerItem3 = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout_8.addItem(spacerItem3)
        self.runoff = QtGui.QLineEdit(self.groupBox_4)
        self.runoff.setAlignment(QtCore.Qt.AlignRight|QtCore.Qt.AlignTrailing|QtCore.Qt.AlignVCenter)
        self.runoff.setObjectName(_fromUtf8("runoff"))
        self.horizontalLayout_8.addWidget(self.runoff)
        self.formLayout.setLayout(0, QtGui.QFormLayout.FieldRole, self.horizontalLayout_8)
        self.horizontalLayout_5.addLayout(self.formLayout)
        self.verticalLayout_3.addWidget(self.groupBox_4)
        self.buttonBox = QtGui.QDialogButtonBox(RationalMethod)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName(_fromUtf8("buttonBox"))
        self.verticalLayout_3.addWidget(self.buttonBox)
        self.labelRunoffCoefficient.setBuddy(self.runoffCoefficient)
        self.labelRainFallIntensity.setBuddy(self.rainfallIntensity)
        self.labelArea.setBuddy(self.watershedArea)
        self.labelRunoff.setBuddy(self.runoff)

        self.retranslateUi(RationalMethod)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("accepted()")), RationalMethod.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("rejected()")), RationalMethod.reject)
        QtCore.QMetaObject.connectSlotsByName(RationalMethod)

    def retranslateUi(self, RationalMethod):
        RationalMethod.setWindowTitle(_translate("RationalMethod", "Rational Formula", None))
        self.groupBox_2.setTitle(_translate("RationalMethod", "Rational formula", None))
        self.labelRunoffCoefficient.setText(_translate("RationalMethod", "Runoff &Coefficient:", None))
        self.getCoefficientButton.setText(_translate("RationalMethod", "...", None))
        self.labelRainFallIntensity.setText(_translate("RationalMethod", "<html><head/><body><p>&amp;Intensity [mm h<span style=\" vertical-align:super;\">-1</span>]:</p></body></html>", None))
        self.labelArea.setText(_translate("RationalMethod", "&Area [ha]:", None))
        self.groupBox_4.setTitle(_translate("RationalMethod", "Runoff", None))
        self.labelRunoff.setText(_translate("RationalMethod", "<html><head/><body><p>&amp;Runoff [m<span style=\" vertical-align:super;\">3</span> s<span style=\" vertical-align:super;\">-1</span>]:</p></body></html>", None))

