import sys
import os
import vtk
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import *
from vtk.util.misc import vtkGetDataRoot
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor
from UI import Ui_MainWindow


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        ##############################################################
        self.Slider = [self.ui.horizontalSlider , self.ui.horizontalSlider_2 , self.ui.horizontalSlider_3 , self.ui.horizontalSlider_4 , self.ui.TissueSlider] 
        self.ui.horizontalSlider.valueChanged.connect(lambda: self.Slider_Changed(0))
        self.ui.horizontalSlider_2.valueChanged.connect(lambda: self.Slider_Changed(1))
        self.ui.horizontalSlider_3.valueChanged.connect(lambda: self.Slider_Changed(2))
        self.ui.horizontalSlider_4.valueChanged.connect(lambda: self.Slider_Changed(3))
        self.ui.TissueSlider.valueChanged.connect(lambda: self.Slider_Changed(4))
        self.ui.LoadDICOM.clicked.connect(lambda: self.Load_DICOM_Series())
        self.ui.comboBox.activated.connect(lambda: self.Rendering_Mode())
        # ##############################################################
        
        self.surfaceExtractor = vtk.vtkContourFilter()
        self.volumeColor = vtk.vtkColorTransferFunction()

        self.RenMD = 0
        self.Flag = False

        self.Edit_UI(0)
    
    ##########################################
    def DICOM_SERIES_PATH(self):
        path = QFileDialog.getExistingDirectory(self, 'Choose DICOM Directory') + '/'
        self.Name = path.split('/')[-2]
        return path 

    ##########################################   
    def Load_DICOM_Series(self):
        self.Flag = True   
        pathDicomDir = None  

        pathDicomDir = self.DICOM_SERIES_PATH()
        self.reader = vtk.vtkDICOMImageReader()
        self.reader.SetDirectoryName(pathDicomDir)
        self.reader.Update()

        if type(pathDicomDir) != 'NoneType':
            self.ui.label_9.setText(self.Name)
            self.Rendering_Mode()

    ##########################################
    def Rendering_Mode(self):
        self.RenMD = self.ui.comboBox.currentIndex() 

        if self.RenMD == 1:
            self.Edit_UI(self.RenMD)
            self.Surface_Rendering()
        elif self.RenMD == 2:
            self.Edit_UI(self.RenMD)
            self.Ray_Casting_Rendering()
    
    ##########################################
    def Slider_Changed(self , ID):
        iren = QVTKRenderWindowInteractor()
        self.Tspt = self.ui.TissueSlider.value() #Tissue Point Changed
        self.ui.label_11.setText(str(self.Tspt))
        
        # Iso_Value
        if ID == 0:
            val = self.Slider[ID].value()
            self.surfaceExtractor.SetValue(0, val)
        
        # Transfer_Func.    
        elif ID == 1:
            val = self.Slider[ID].value()/100.0
            self.ui.label_6.setText(str(val))

            GVal=self.volumeColor.GetGreenValue(self.Tspt) 
            BVal=self.volumeColor.GetBlueValue(self.Tspt)
            self.volumeColor.RemovePoint(self.Tspt)
            self.volumeColor.AddRGBPoint(self.Tspt,  val, GVal , BVal)
               
        elif ID == 2:
            val = self.Slider[ID].value()/100.0
            self.ui.label_7.setText(str(val))
        
            RVal=self.volumeColor.GetRedValue(self.Tspt)
            BVal=self.volumeColor.GetBlueValue(self.Tspt)
            self.volumeColor.RemovePoint(self.Tspt)
            self.volumeColor.AddRGBPoint(self.Tspt,  RVal, val , BVal)

        elif ID == 3:
            val = self.Slider[ID].value()/100.0
            self.ui.label_8.setText(str(val))
            
            RVal=self.volumeColor.GetRedValue(self.Tspt)
            GVal=self.volumeColor.GetGreenValue(self.Tspt) 
            self.volumeColor.RemovePoint(self.Tspt)
            self.volumeColor.AddRGBPoint(self.Tspt,  RVal, GVal , val)

        iren.update()

    ##########################################            
    def Surface_Rendering(self):
        if self.Flag == True : 
            iren = QVTKRenderWindowInteractor()
            self.viewer = iren.GetRenderWindow()
            self.aRenderer = vtk.vtkRenderer()
            self.viewer.AddRenderer(self.aRenderer) 

            self.surfaceExtractor.SetInputConnection(self.reader.GetOutputPort())
            self.surfaceExtractor.SetValue(0, 500)
            self.surfaceNormals = vtk.vtkPolyDataNormals()
            self.surfaceNormals.SetInputConnection(self.surfaceExtractor.GetOutputPort())
            self.surfaceNormals.SetFeatureAngle(60.0)
            self.surfaceMapper = vtk.vtkPolyDataMapper()
            self.surfaceMapper.SetInputConnection(self.surfaceNormals.GetOutputPort())
            self.surfaceMapper.ScalarVisibilityOff()

            surface = vtk.vtkActor()
            surface.SetMapper(self.surfaceMapper)
        
            aCamera = vtk.vtkCamera()
            aCamera.SetViewUp(0, 0, -1)
            aCamera.SetPosition(0, 1, 0)
            aCamera.SetFocalPoint(0, 0, 0)
            aCamera.ComputeViewPlaneNormal()

            self.aRenderer.AddActor(surface)
            self.aRenderer.SetActiveCamera(aCamera)
            self.aRenderer.ResetCamera()
            aCamera.Dolly(1)

            self.aRenderer.SetBackground(0, 0, 0)
            self.viewer.SetSize(640, 480)
            self.aRenderer.ResetCameraClippingRange()

            iren.Initialize()
            self.viewer.Render()
            iren.Start()
            iren.show()
            
    ##########################################
    def Ray_Casting_Rendering(self):

        if self.Flag == True:
            iren = QVTKRenderWindowInteractor()
            renWin = iren.GetRenderWindow()
            ren = vtk.vtkRenderer()
            renWin.AddRenderer(ren) 

            volumeMapper = vtk.vtkGPUVolumeRayCastMapper()
            volumeMapper.SetInputConnection(self.reader.GetOutputPort())
            volumeMapper.SetBlendModeToComposite()

            self.volumeColor = vtk.vtkColorTransferFunction()
            self.volumeColor.AddRGBPoint(0,    0.0, 0.0, 0.0)
            self.volumeColor.AddRGBPoint(500,  1.0, 0.5, 0.3)
            self.volumeColor.AddRGBPoint(1000,  1.0, 0.5, 0.3)
            self.volumeColor.AddRGBPoint(1150, 1.0, 1.0, 0.9)
        
            volumeScalarOpacity = vtk.vtkPiecewiseFunction()
            volumeScalarOpacity.AddPoint(0,    0.00)
            volumeScalarOpacity.AddPoint(500,  1.0)
            volumeScalarOpacity.AddPoint(1000, 0.7)
            volumeScalarOpacity.AddPoint(1150, 0.03)
        
            volumeGradientOpacity = vtk.vtkPiecewiseFunction()
            volumeGradientOpacity.AddPoint(0,   0.0)
            volumeGradientOpacity.AddPoint(90,  0.5)
            volumeGradientOpacity.AddPoint(100, 1.0)

            volumeProperty = vtk.vtkVolumeProperty()
            volumeProperty.SetColor(self.volumeColor)
            volumeProperty.SetScalarOpacity(volumeScalarOpacity)
            volumeProperty.SetGradientOpacity(volumeGradientOpacity)
            volumeProperty.SetInterpolationTypeToLinear()
            volumeProperty.ShadeOn()
            volumeProperty.SetAmbient(0.4)
            volumeProperty.SetDiffuse(0.6)
            volumeProperty.SetSpecular(0.2)
    
            volume = vtk.vtkVolume()
            volume.SetMapper(volumeMapper)
            volume.SetProperty(volumeProperty)

            ren.AddViewProp(volume)

            camera =  ren.GetActiveCamera()
            c = volume.GetCenter()
            camera.SetFocalPoint(c[0], c[1], c[2])
            camera.SetPosition(c[0] + 600, c[1], c[2])
            camera.SetViewUp(0, 0, -1)

            renWin.SetSize(640, 480)

            iren.Initialize()
            renWin.Render()
            iren.Start()
            iren.show()

    ##########################################
    def Edit_UI (self , RenMD):
        if RenMD == 0:
            self.ui.horizontalSlider.hide()
            self.ui.horizontalSlider_2.hide()
            self.ui.horizontalSlider_3.hide()
            self.ui.horizontalSlider_4.hide()
            self.ui.TissueSlider.hide()
            self.ui.label.hide()
            self.ui.label_2.hide()
            self.ui.label_3.hide()
            self.ui.label_4.hide()
            self.ui.label_5.hide()
            self.ui.label_6.hide()
            self.ui.label_7.hide()
            self.ui.label_8.hide()
            self.ui.label_11.hide()
            self.ui.TissueLabel.hide()
            MainWindow.resize(self, 529, 115)

        if RenMD == 1:
            self.ui.horizontalSlider.show()
            self.ui.horizontalSlider_2.hide()
            self.ui.horizontalSlider_3.hide()
            self.ui.horizontalSlider_4.hide()
            self.ui.TissueSlider.hide()
            self.ui.label.show()
            self.ui.label_2.show()
            self.ui.label_3.hide()
            self.ui.label_4.hide()
            self.ui.label_5.hide()
            self.ui.label_6.hide()
            self.ui.label_7.hide()
            self.ui.label_8.hide()
            self.ui.label_11.hide()
            self.ui.TissueLabel.hide()
            MainWindow.resize(self, 529, 230)

        elif RenMD== 2:
            self.ui.horizontalSlider.hide()
            self.ui.horizontalSlider_2.show()
            self.ui.horizontalSlider_3.show()
            self.ui.horizontalSlider_4.show()
            self.ui.TissueSlider.show()
            self.ui.label.hide()
            self.ui.label_2.hide()
            self.ui.label_3.show()
            self.ui.label_4.show()
            self.ui.label_5.show()
            self.ui.label_6.show()
            self.ui.label_7.show()
            self.ui.label_8.show()
            self.ui.label_11.show()
            self.ui.TissueLabel.show()
            MainWindow.resize(self, 529, 490)

def main():
    path = os.getcwd()
    os.chdir(path + '/Data')
    directory = os.getcwd()
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
    w.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
