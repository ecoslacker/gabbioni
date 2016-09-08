# -*- coding: utf-8 -*-
"""
Created on Thu Sep 11 11:52:01 2014

@author: ecoslacker
"""
import sys
from PyQt4 import QtCore, QtGui
# import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
from GabbioniUi import Ui_MainWindow, Ui_SimpleDesign, Ui_RationalMethod, Ui_AboutDlg, Ui_RunoffCoefficient
from lib.gdamslib import DataFile, RationalFormula, Weir, GabionDam, RunoffCoefficient


class GabbioniMainWindow(QtGui.QMainWindow, Ui_MainWindow):
    """ The main window of the program """

    def __init__(self, parent=None):

        super(GabbioniMainWindow, self).__init__(parent)
        self.setupUi(self)

        #        self.mplwidget.figure.subplotpars.left = 0.1
        #        self.mplwidget.figure.subplotpars.right = 0.1
        #        self.mplwidget.figure.subplotpars.top = 0.1
        #        self.mplwidget.figure.subplotpars.bottom = 0.1
        #        self.mplwidget.figure.subplotpars.wspace=0.2
        #        self.mplwidget.figure.subplotpars.hspace=0.2

        self.mplwidget.axes.clear()
        self.mplwidget.axes.grid(True)
        self.mplwidget.axes.set_aspect('equal')
        self.mplwidget.axes.set_adjustable('datalim')

        # Set the Y-Axis inverted
        self.mplwidget.axes.invert_yaxis()

        # Tight layout, minimize the figure margins
        self.mplwidget.figure.tight_layout()

        self.mplwidget.axes.set_title(u"Front view")
        self.set_front_titles()

        # Colors
        self.mplwidget.figure.set_facecolor('k')
        self.mplwidget.axes.set_axis_bgcolor('k')
        self.mplwidget.axes.spines['bottom'].set_color('w')
        self.mplwidget.axes.spines['top'].set_color('w')
        self.mplwidget.axes.spines['right'].set_color('w')
        self.mplwidget.axes.spines['left'].set_color('w')
        self.mplwidget.axes.tick_params(axis='x', colors='w')
        self.mplwidget.axes.tick_params(axis='y', colors='w')
        self.mplwidget.axes.xaxis.label.set_color('w')
        self.mplwidget.axes.yaxis.label.set_color('w')
        self.mplwidget.axes.title.set_color('w')

        # connect the signals with the slots
        self.actionNew.triggered.connect(self.new_project)
        self.actionOpen.triggered.connect(self.open_file)
        self.actionAbout.triggered.connect(self.about_dialog)
        self.actionSimpleDesign.triggered.connect(self.simple_design_dialog)
        self.actionLayerByLayerDesign.triggered.connect(self.step_design_dialog)

        # Define other objects
        self.about_dlg = QtGui.QDialog()
        self.simple_dlg = QtGui.QDialog()
        self.rational_dlg = QtGui.QDialog()
        self.runoff_coeff_dlg = QtGui.QDialog()

        # Choose a layer's height
        heights = [0.15, 0.30, 0.5, 1.0]

        # Defaults
        self.layer_height = heights[2]
        self.weir_water_level = 0
        self.weir_height = 0
        self.weir_layers = 0
        self.weir_coef = 1.45  # Coefficient for a rectangular weir, same as self.coefficient
        self.weir_len = 2
        self.weir_notch = 'h'
        self.abutment = 0.5
        self.runoff = 0. # same as self.flowrate
        self.runoff_coefficient = 0.

        # Create a new instance of the gabion dam class, using default parameters
        # WARNING! These parameters should be updated later
        self.dam = GabionDam([], self.layer_height, self.runoff, self.weir_coef,
                             self.weir_len, self.weir_height, self.weir_notch)

    def closeEvent(self, event):

        """ Close event.
        
        A message that asks for quit the program.        
        Close event defined by Python language and Qt itself.
        
        """

        answer = QtGui.QMessageBox.question(self, 'Quit',
                                            u'Quit Gabbioni application?',
                                            QtGui.QMessageBox.Yes |
                                            QtGui.QMessageBox.No,
                                            QtGui.QMessageBox.No)
        if answer == QtGui.QMessageBox.Yes:
            event.accept()
        else:
            event.ignore()

    def about_dialog(self):

        self.about_dlg = QtGui.QDialog()
        self.about_dlg.ui = Ui_AboutDlg()
        self.about_dlg.ui.setupUi(self.about_dlg)
        self.about_dlg.setAttribute(QtCore.Qt.WA_DeleteOnClose)
        self.about_dlg.exec_()

    def set_front_titles(self):

        # self.mplwidget.axes.set_title(u"Front view")
        self.mplwidget.axes.set_xlabel(u"Width (m)")
        self.mplwidget.axes.set_ylabel(u"Depth (m)")

    def new_project(self):

        self.new_project_dlg = QtGui.QMessageBox()
        self.new_project_dlg.setWindowTitle("New project")
        self.new_project_dlg.setText("The new project has been created & its ready!")
        self.new_project_dlg.exec_()

        # answer = QtGui.QMessageBox.question(self, 'New Project',
        #                                     u'New project ready!',
        #                                     QtGui.QMessageBox.Ok,
        #                                     QtGui.QMessageBox.Ok)

    def open_file(self):

        """ Open file
        
        Open data of the channel cross section of the stream 
        from a file in Comma Separated Values (CSV) format.
        
        """

        file_name = unicode(QtGui.QFileDialog.getOpenFileName(self,
                                                              "Open data from file",
                                                              QtCore.QDir.homePath(),
                                                              "CSV (Comma delimited) (*.csv)"))

        print("Opening file: '{0}'".format(file_name))

        try:
            # Read the data from text file
            filedata = DataFile(file_name)
            filedata.read_data()
            if filedata.check_data() is True:
                # Plot the data
                self.plot_data(filedata.data)
            else:
                print("There are errors in the data.")
        except IOError, e:
            print("IOError: {0}".format(e))

    def plot_data(self, data):

        # Set the passed data to the main gabion dam instance
        self.dam.data = data

        x = []
        y = []

        # Set the data to the arrays of coordinates
        x, y = self.dam.get_xy()
        print(x)
        print(y)

        # Prepare for plotting the new data
        self.mplwidget.axes.clear()

        # Plot the data with dots and lines
        self.mplwidget.axes.plot(x, y, 'b.-')

        self.set_front_titles()
        # self.mplwidget.axes.set_title(u"Front view")

        # Draw the mplwidget to see the patch recently added
        self.mplwidget.draw()

        self.mplwidget.axes.set_title(u"Front view")

    def simple_design_dialog(self):

        self.simple_dlg = QtGui.QDialog()
        self.simple_dlg.ui = Ui_SimpleDesign()
        self.simple_dlg.ui.setupUi(self.simple_dlg)
        self.simple_dlg.setAttribute(QtCore.Qt.WA_DeleteOnClose)

        # Open dialog for runoff calculation
        self.simple_dlg.ui.runoffRationalFormula.pressed.connect(self.rational_dialog)

        # Perform weir dimensioning when the user change the text of the
        # line edit widgets or the index of the combo box
        self.simple_dlg.ui.weirLenght.textChanged.connect(self.weir_dimensioning)
        self.simple_dlg.ui.weirCoefficient.textChanged.connect(self.weir_dimensioning)
        self.simple_dlg.ui.weirLayersHeight.currentIndexChanged.connect(self.weir_dimensioning)

        # Set the abutments properties
        self.simple_dlg.ui.abutmentCustom.clicked.connect(self.set_abutment)
        self.simple_dlg.ui.abutmentEqual.clicked.connect(self.set_abutment)
        self.simple_dlg.ui.abutmentValue.textChanged.connect(self.set_abutment)

        self.simple_dlg.exec_()

        # If this dialog is Accepted go ahead with the dam design
        if QtGui.QDialog.Accepted:
            # TODO: Check the input data
            print("Performing a data check")

            # Start dam design
            print("Starting the dam design")
            self.design_dam()

    def step_design_dialog(self):

        self.step_design_dialog = QtGui.QMessageBox()
        self.step_design_dialog.setWindowTitle("Design")
        self.step_design_dialog.setText("This function has not been implemented in this version yet!")
        self.step_design_dialog.exec_()

    def rational_dialog(self):

        self.rational_dlg = QtGui.QDialog()
        self.rational_dlg.ui = Ui_RationalMethod()
        self.rational_dlg.ui.setupUi(self.rational_dlg)
        self.rational_dlg.setAttribute(QtCore.Qt.WA_DeleteOnClose)

        # Perform runoff calculation using rational formula when the user
        # changes the text of the line edits widgets
        self.rational_dlg.ui.runoffCoefficient.textChanged.connect(self.rational_method)
        self.rational_dlg.ui.rainfallIntensity.textChanged.connect(self.rational_method)
        self.rational_dlg.ui.watershedArea.textChanged.connect(self.rational_method)
        self.rational_dlg.ui.getCoefficientButton.pressed.connect(self.runoff_coefficient_dialog)

        self.rational_dlg.exec_()

        # If this dialog is Accepted, put the runoff value in the Simple
        # Design dialog to continue the dam design process
        if QtGui.QDialog.Accepted:
            print("Setting runoff")
            self.simple_dlg.ui.runoff.setText("{0:.4f}".format(self.runoff))

    def rational_method(self):
        """ Runoff using Rational formula
            
            Calculation of watershed runoff using the Rational formula,
            (which is a simple multiplication).
            
            """

        try:
            # Get data from UI
            c = float(self.rational_dlg.ui.runoffCoefficient.text())
            i = float(self.rational_dlg.ui.rainfallIntensity.text())
            a = float(self.rational_dlg.ui.watershedArea.text())

            # Calculate watershed runoff using rational formula
            rational_formula = RationalFormula()
            rational_formula.coefficient = c
            rational_formula.rainfall = i
            rational_formula.area = a

            # Everything is OK so far so
            print("Running rational formula")
            self.runoff = rational_formula.get_runoff()

            self.rational_dlg.ui.runoff.setText("{0:.4f}".format(self.runoff))

            print(" Runoff: {0}".format(self.runoff))

        except ValueError, e:

            print(" Error: {0}".format(e))

    def runoff_coefficient_dialog(self):

        self.runoff_coeff_dlg = QtGui.QDialog()
        self.runoff_coeff_dlg.ui = Ui_RunoffCoefficient()
        self.runoff_coeff_dlg.ui.setupUi(self.runoff_coeff_dlg)
        self.runoff_coeff_dlg.setAttribute(QtCore.Qt.WA_DeleteOnClose)

        self.runoff_coeff_dlg.ui.tableWidget.setColumnWidth(0, 290)
        self.runoff_coeff_dlg.ui.tableWidget.setColumnWidth(1, 50)
        self.runoff_coeff_dlg.ui.tableWidget.setColumnWidth(2, 50)
        self.runoff_coeff_dlg.ui.tableWidget.setColumnWidth(3, 50)

        self.runoff_coeff_dlg.ui.tableWidget.cellClicked.connect(self.get_parameter_k)
        self.runoff_coeff_dlg.ui.annualPrecipitation.textEdited.connect(self.get_runoff_coefficient)

        self.runoff_coeff_dlg.exec_()

        # If this dialog is Accepted, put the runoff coefficient value in the
        # Rational dialog to continue the dam design process
        if QtGui.QDialog.Accepted:
            print("Setting runoff coefficient")
            self.rational_dlg.ui.runoffCoefficient.setText("{0:.4f}".format(self.runoff_coefficient))

    def get_parameter_k(self, r, c):
        """ Parameter K selected from table used for runoff coefficient. """

        print('  Row: %d' % r)
        print('  Col: %d' % c)
        try:
            item = self.runoff_coeff_dlg.ui.tableWidget.item(r, c)
            if c != 0:
                print('  Value: %s' % item.text())
                self.param_k = float(item.text())

                # Calculate runoff coefficient from here too
                self.get_runoff_coefficient()
        except AttributeError:
            self.param_k = -1
            print("  Empty cell selected.")
        # except ValueError:
        #     print("Could not convert string to float.")

    def get_runoff_coefficient(self):

        print('  K: %s' % str(self.param_k))
        if self.param_k != -1:
            # Calculate runoff coefficient
            annual_precip = float(self.runoff_coeff_dlg.ui.annualPrecipitation.text())
            print('  Annual precipitation: %s' % str(annual_precip))

            # Create an instance of the RunoffCoefficient class to calculate its value
            runoff_obj = RunoffCoefficient(0, 0, annual_precip)  # Just initialize with something
            self.runoff_coefficient = runoff_obj.calc_runoff_coefficient(self.param_k, annual_precip)

            # Set the runoff coefficient to its place
            self.runoff_coeff_dlg.ui.runoffCoefficient.setText("{0:.4f}".format(self.runoff_coefficient))
        else:
            self.runoff_coefficient = 0

    def weir_dimensioning(self):
        """ Weir dimensioning
        
        Calculation of the weir dimensions.
        """

        try:
            # Get data from UI
            q = float(self.simple_dlg.ui.runoff.text())
            c = float(self.simple_dlg.ui.weirCoefficient.text())
            l = float(self.simple_dlg.ui.weirLenght.text())
            # Get the height of the gabion
            step = float(self.simple_dlg.ui.weirLayersHeight.currentText())

            # print("Running weir dimensioning")

            # Calculate water head

            # weir = Weir(q, c, l, 0, 'h')
            print(self.dam.flowrate)
            print(self.dam.weir_coefficient)

            self.dam.flowrate = q
            self.dam.weir_coefficient = c
            self.dam.weir_length = l
            self.dam.weir_height = 0
            self.dam.weir_notch = 'h'
            self.dam.layers_height = step
            h = self.dam.get_head()

            # Calculate weir height
            wh = self.dam.get_height()
            ly = self.dam.weir_layers

            # Set water height to UI
            self.simple_dlg.ui.waterLevel.setText("{0:.4f}".format(h))

            # Set weir height to UI
            self.simple_dlg.ui.weirHeight.setText("{0:.2f}".format(wh))

            # Set the number gabion layers for the weir
            self.simple_dlg.ui.weirLayers.setText("{0}".format(ly))

            # self.weir_water_level = h
            # self.weir_height = wh
            # self.weir_layers = ly

            # print("-Weir dimensions:")
            # print(" Water head: {0}".format(h))
            # print(" Weir height: {0}".format(wh))
            # print(" Weir layers: {0}".format(ly))

        except ValueError, e:

            print(" Error: {0}".format(e))

    def set_abutment(self):

        try:

            if self.simple_dlg.ui.abutmentEqual.isChecked():
                self.simple_dlg.ui.abutmentValue.setEnabled(False)
                self.abutment = float(self.simple_dlg.ui.bodyLayersHeight.currentText())
            else:
                self.simple_dlg.ui.abutmentValue.setEnabled(True)
                self.abutment = float(self.simple_dlg.ui.abutmentValue.text())

            print("Setting abutment to: {0:.2f}".format(self.abutment))

        except ValueError, e:

            print(" Error: {0}".format(e))

    def design_dam(self):
        """ Designs the dam

        Gets the dimensions of each layer of the dam body and the weir, and
        plots the layers obtained. Uses a simple algorithm for this approach.
        """

        print(" Design dam")

        self.dam.get_xy()
        self.dam.get_widths()
        self.dam.show_widths()
        self.dam.get_head()
        self.dam.auto_dimensioning()
        # self.dam.show_dimensions()
        self.dam.dimension_weir()

        self.front_view()

    # TODO:
    # Pass the main canvas to the plotting fuctions as an argument and
    # delete this functions
    def front_view(self):

        """ Draws the front view of the dam. """

        print("Plotting front view")

        # Plot the channel's cross section
        # self.plot_data(self.dam.data)

        print(self.dam.dimensions)

        # Create each layer of the dam body
        for i in range(len(self.dam.dimensions)):
            # Display the coordinates and the dimensions of the current layer
            print(" X={0}".format(self.dam.dimensions[i][3]))
            print(" Y={0}".format(self.dam.dimensions[i][4]))
            print(" Width={0}".format(self.dam.dimensions[i][1]))
            print(" Height={0}".format(self.dam.dimensions[i][2]))

            left = self.dam.dimensions[i][3]
            right = self.dam.dimensions[i][3] + self.dam.dimensions[i][1]

            # Because the inverted Y-Axis bottom and top should be as follow
            top = self.dam.dimensions[i][4]
            bottom = self.dam.dimensions[i][4] - self.dam.dimensions[i][2]

            # Using a PathPatch
            verts = [(left, bottom),
                     (left, top),
                     (right, top),
                     (right, bottom),
                     (left, bottom)]
            codes = [Path.MOVETO,
                     Path.LINETO,
                     Path.LINETO,
                     Path.LINETO,
                     Path.CLOSEPOLY]
            path = Path(verts, codes)
            patch = patches.PathPatch(path, edgecolor='white', alpha=0.75,
                                      facecolor='0.75', lw=1, hatch='x')

            # Using a Rectangle patch 
            # WARNING! Adjust the coordinates of the lower-left corner
            # in order to display the rectangle properly
            #            patch = patches.Rectangle((self.dam.dimensions[i][3],
            #                                      self.dam.dimensions[i][4]),
            #                                     self.dam.dimensions[i][1],
            #                                     self.dam.dimensions[i][2],
            #                                     color = 'w', ec = 'w')

            self.mplwidget.axes.add_patch(patch)

            self.set_front_titles()
            self.mplwidget.axes.set_title(u"Front view")

            # Draw the mplwidget to see the patch recently added
            self.mplwidget.draw()

    def stability_analysis(self):
        """ Stability analysis
        
        Performs the stability analysis with the dimensions of the dam.
        """

        # Check the dimensions table


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    app.processEvents()

    gabbioni = GabbioniMainWindow()
    gabbioni.show()
    QtGui.QApplication.instance().processEvents()

    app.exec_()
