#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Run Music Lin Array Simulation
# Generated: Mon Aug  6 19:40:48 2018
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

def struct(data): return type('Struct', (object,), data)()
from PyQt4 import Qt
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import doa
import numpy
import sip
import sys


class run_MUSIC_lin_array_simulation(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Run Music Lin Array Simulation")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Run Music Lin Array Simulation")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "run_MUSIC_lin_array_simulation")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.theta1_deg = theta1_deg = 123
        self.theta0_deg = theta0_deg = 30
        self.input_variables = input_variables = struct({'SampleRate': 320000, 'ToneFreq1': 10000, 'ToneFreq2': 20000, 'NormSpacing': 0.4, 'NumTargets': 2, 'NumArrayElements': 4, 'PSpectrumLength': 2**10, 'SnapshotSize': 2**11, 'OverlapSize': 2**9, })
        self.theta1 = theta1 = numpy.pi*theta1_deg/180
        self.theta0 = theta0 = numpy.pi*theta0_deg/180
        self.ant_locs = ant_locs = numpy.dot(input_variables.NormSpacing, numpy.arange(input_variables.NumArrayElements/2, -input_variables.NumArrayElements/2, -1) if (input_variables.NumArrayElements%2==1) else numpy.arange(input_variables.NumArrayElements/2-0.5, -input_variables.NumArrayElements/2-0.5, -1))
        self.amv1 = amv1 = numpy.exp(-1j*ant_locs*2*numpy.pi*numpy.cos(theta1))
        self.amv0 = amv0 = numpy.exp(-1j*ant_locs*2*numpy.pi*numpy.cos(theta0))
        self.array_manifold_matrix = array_manifold_matrix = numpy.array([amv0, amv1]).transpose()

        ##################################################
        # Blocks
        ##################################################
        self.tab = Qt.QTabWidget()
        self.tab_widget_0 = Qt.QWidget()
        self.tab_layout_0 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_widget_0)
        self.tab_grid_layout_0 = Qt.QGridLayout()
        self.tab_layout_0.addLayout(self.tab_grid_layout_0)
        self.tab.addTab(self.tab_widget_0, 'Pseudo-Spectrum')
        self.tab_widget_1 = Qt.QWidget()
        self.tab_layout_1 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_widget_1)
        self.tab_grid_layout_1 = Qt.QGridLayout()
        self.tab_layout_1.addLayout(self.tab_grid_layout_1)
        self.tab.addTab(self.tab_widget_1, 'Direction of Arrival')
        self.tab_widget_2 = Qt.QWidget()
        self.tab_layout_2 = Qt.QBoxLayout(Qt.QBoxLayout.TopToBottom, self.tab_widget_2)
        self.tab_grid_layout_2 = Qt.QGridLayout()
        self.tab_layout_2.addLayout(self.tab_grid_layout_2)
        self.tab.addTab(self.tab_widget_2, 'Direction of Arrival')
        self.top_layout.addWidget(self.tab)
        self._theta1_deg_range = Range(0, 180, 1, 123, 200)
        self._theta1_deg_win = RangeWidget(self._theta1_deg_range, self.set_theta1_deg, 'AoA', "counter_slider", float)
        self.top_layout.addWidget(self._theta1_deg_win)
        self._theta0_deg_range = Range(0, 180, 1, 30, 200)
        self._theta0_deg_win = RangeWidget(self._theta0_deg_range, self.set_theta0_deg, 'AoA', "counter_slider", float)
        self.top_layout.addWidget(self._theta0_deg_win)
        self.qtgui_vector_sink_f_0 = qtgui.vector_sink_f(
            input_variables.PSpectrumLength,
            0,
            180.0/input_variables.PSpectrumLength,
            "angle (in degrees)",
            "Pseudo-Spectrum (dB)",
            "",
            1 # Number of inputs
        )
        self.qtgui_vector_sink_f_0.set_update_time(0.05)
        self.qtgui_vector_sink_f_0.set_y_axis(-50, 0)
        self.qtgui_vector_sink_f_0.enable_autoscale(False)
        self.qtgui_vector_sink_f_0.enable_grid(True)
        self.qtgui_vector_sink_f_0.set_x_axis_units("")
        self.qtgui_vector_sink_f_0.set_y_axis_units("")
        self.qtgui_vector_sink_f_0.set_ref_level(0)
        
        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [2, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_vector_sink_f_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_vector_sink_f_0.set_line_label(i, labels[i])
            self.qtgui_vector_sink_f_0.set_line_width(i, widths[i])
            self.qtgui_vector_sink_f_0.set_line_color(i, colors[i])
            self.qtgui_vector_sink_f_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_vector_sink_f_0_win = sip.wrapinstance(self.qtgui_vector_sink_f_0.pyqwidget(), Qt.QWidget)
        self.tab_layout_0.addWidget(self._qtgui_vector_sink_f_0_win)
        self.doa_find_local_max_0 = doa.find_local_max(input_variables.NumTargets, input_variables.PSpectrumLength, 0.0, 180.0)
        self.doa_compass_0 = doa.compass("", 0, 180, 10, 0)
        self.tab_layout_1.addLayout(self.doa_compass_0.this_layout)
        self.doa_compass = doa.compass("", 0, 180, 10, 0)
        self.tab_layout_2.addLayout(self.doa_compass.this_layout)
        self.doa_autocorrelate_0 = doa.autocorrelate(input_variables.NumArrayElements, input_variables.SnapshotSize, input_variables.OverlapSize, 1)
        self.doa_MUSIC_lin_array_0 = doa.MUSIC_lin_array(input_variables.NormSpacing, input_variables.NumTargets, input_variables.NumArrayElements, input_variables.PSpectrumLength)
        self.blocks_vector_to_streams_0 = blocks.vector_to_streams(gr.sizeof_float*1, input_variables.NumTargets)
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_gr_complex*1, input_variables.SampleRate,True)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_float*input_variables.NumTargets)
        self.blocks_multiply_matrix_xx_0 = blocks.multiply_matrix_cc(array_manifold_matrix, gr.TPP_ALL_TO_ALL)
        self.blocks_add_xx_0_0 = blocks.add_vcc(1)
        self.blocks_add_xx_0 = blocks.add_vcc(1)
        self.analog_sig_source_x_0_0 = analog.sig_source_c(input_variables.SampleRate, analog.GR_COS_WAVE, input_variables.ToneFreq2, 1, 0)
        self.analog_sig_source_x_0 = analog.sig_source_c(input_variables.SampleRate, analog.GR_COS_WAVE, input_variables.ToneFreq1, 1, 0)
        self.analog_noise_source_x_0_0_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.5, 0)
        self.analog_noise_source_x_0_0 = analog.noise_source_c(analog.GR_GAUSSIAN, 0.0005, 0)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_noise_source_x_0_0, 0), (self.blocks_add_xx_0, 1))    
        self.connect((self.analog_noise_source_x_0_0_0, 0), (self.blocks_add_xx_0_0, 1))    
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_add_xx_0, 0))    
        self.connect((self.analog_sig_source_x_0_0, 0), (self.blocks_add_xx_0_0, 0))    
        self.connect((self.blocks_add_xx_0, 0), (self.blocks_multiply_matrix_xx_0, 0))    
        self.connect((self.blocks_add_xx_0_0, 0), (self.blocks_throttle_0_0, 0))    
        self.connect((self.blocks_multiply_matrix_xx_0, 0), (self.doa_autocorrelate_0, 0))    
        self.connect((self.blocks_multiply_matrix_xx_0, 1), (self.doa_autocorrelate_0, 1))    
        self.connect((self.blocks_multiply_matrix_xx_0, 2), (self.doa_autocorrelate_0, 2))    
        self.connect((self.blocks_multiply_matrix_xx_0, 3), (self.doa_autocorrelate_0, 3))    
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_multiply_matrix_xx_0, 1))    
        self.connect((self.blocks_vector_to_streams_0, 0), (self.doa_compass, 0))    
        self.connect((self.blocks_vector_to_streams_0, 1), (self.doa_compass_0, 0))    
        self.connect((self.doa_MUSIC_lin_array_0, 0), (self.doa_find_local_max_0, 0))    
        self.connect((self.doa_MUSIC_lin_array_0, 0), (self.qtgui_vector_sink_f_0, 0))    
        self.connect((self.doa_autocorrelate_0, 0), (self.doa_MUSIC_lin_array_0, 0))    
        self.connect((self.doa_find_local_max_0, 0), (self.blocks_null_sink_0, 0))    
        self.connect((self.doa_find_local_max_0, 1), (self.blocks_vector_to_streams_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "run_MUSIC_lin_array_simulation")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_theta1_deg(self):
        return self.theta1_deg

    def set_theta1_deg(self, theta1_deg):
        self.theta1_deg = theta1_deg
        self.set_theta1(numpy.pi*self.theta1_deg/180)

    def get_theta0_deg(self):
        return self.theta0_deg

    def set_theta0_deg(self, theta0_deg):
        self.theta0_deg = theta0_deg
        self.set_theta0(numpy.pi*self.theta0_deg/180)

    def get_input_variables(self):
        return self.input_variables

    def set_input_variables(self, input_variables):
        self.input_variables = input_variables

    def get_theta1(self):
        return self.theta1

    def set_theta1(self, theta1):
        self.theta1 = theta1
        self.set_amv1(numpy.exp(-1j*self.ant_locs*2*numpy.pi*numpy.cos(self.theta1)))

    def get_theta0(self):
        return self.theta0

    def set_theta0(self, theta0):
        self.theta0 = theta0
        self.set_amv0(numpy.exp(-1j*self.ant_locs*2*numpy.pi*numpy.cos(self.theta0)))

    def get_ant_locs(self):
        return self.ant_locs

    def set_ant_locs(self, ant_locs):
        self.ant_locs = ant_locs
        self.set_amv1(numpy.exp(-1j*self.ant_locs*2*numpy.pi*numpy.cos(self.theta1)))
        self.set_amv0(numpy.exp(-1j*self.ant_locs*2*numpy.pi*numpy.cos(self.theta0)))

    def get_amv1(self):
        return self.amv1

    def set_amv1(self, amv1):
        self.amv1 = amv1
        self.set_array_manifold_matrix(numpy.array([self.amv0, self.amv1]).transpose())

    def get_amv0(self):
        return self.amv0

    def set_amv0(self, amv0):
        self.amv0 = amv0
        self.set_array_manifold_matrix(numpy.array([self.amv0, self.amv1]).transpose())

    def get_array_manifold_matrix(self):
        return self.array_manifold_matrix

    def set_array_manifold_matrix(self, array_manifold_matrix):
        self.array_manifold_matrix = array_manifold_matrix
        self.blocks_multiply_matrix_xx_0.set_A(self.array_manifold_matrix)


def main(top_block_cls=run_MUSIC_lin_array_simulation, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
