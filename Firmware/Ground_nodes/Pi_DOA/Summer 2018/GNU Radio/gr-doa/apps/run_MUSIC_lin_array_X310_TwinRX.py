#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Run Music Lin Array X310 Twinrx
# Generated: Mon Aug  6 19:40:24 2018
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
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import doa
import os
import sip
import sys


class run_MUSIC_lin_array_X310_TwinRX(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Run Music Lin Array X310 Twinrx")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Run Music Lin Array X310 Twinrx")
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

        self.settings = Qt.QSettings("GNU Radio", "run_MUSIC_lin_array_X310_TwinRX")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.input_variables = input_variables = struct({'ToneFreq': 10000, 'SampleRate': 1000000, 'CenterFreq': 2450000000, 'RxAddr': "addr=192.168.40.2", 'Gain': 60, 'NumArrayElements': 4, 'NormSpacing': 0.5, 'SnapshotSize': 2**11, 'OverlapSize': 2**9, 'NumTargets': 1, 'PSpectrumLength': 2**10, 'DirectoryConfigFiles': "/tmp", 'RelativePhaseOffsets': "measure_X310_TwinRX_relative_phase_offsets_245.cfg", })
        self.rel_phase_offsets_file_name = rel_phase_offsets_file_name = os.path.join(input_variables.DirectoryConfigFiles, input_variables.RelativePhaseOffsets)

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
        self.tab.addTab(self.tab_widget_1, 'Angle of Arrival (MUSIC)')
        self.top_layout.addWidget(self.tab)
        self.twinrx_usrp_source_0 = doa.twinrx_usrp_source(
            samp_rate=input_variables.SampleRate,
            center_freq=input_variables.CenterFreq,
            gain=input_variables.Gain,
            sources=input_variables.NumArrayElements,
            addresses=input_variables.RxAddr
        )
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
        self.qtgui_vector_sink_f_0.set_y_axis(-20, 0)
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
        self.phase_correct_hier_1 = doa.phase_correct_hier(
            num_ports=input_variables.NumArrayElements,
            config_filename=rel_phase_offsets_file_name,
        )
        self.doa_find_local_max_0 = doa.find_local_max(input_variables.NumTargets, input_variables.PSpectrumLength, 0.0, 180.0)
        self.doa_compass = doa.compass("", 0, 180, 10, 0)
        self.tab_layout_1.addLayout(self.doa_compass.this_layout)
        self.doa_autocorrelate_0 = doa.autocorrelate(input_variables.NumArrayElements, input_variables.SnapshotSize, input_variables.OverlapSize, 1)
        self.doa_MUSIC_lin_array_0 = doa.MUSIC_lin_array(input_variables.NormSpacing, input_variables.NumTargets, input_variables.NumArrayElements, input_variables.PSpectrumLength)
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_float*input_variables.NumTargets)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.doa_MUSIC_lin_array_0, 0), (self.doa_find_local_max_0, 0))    
        self.connect((self.doa_MUSIC_lin_array_0, 0), (self.qtgui_vector_sink_f_0, 0))    
        self.connect((self.doa_autocorrelate_0, 0), (self.doa_MUSIC_lin_array_0, 0))    
        self.connect((self.doa_find_local_max_0, 0), (self.blocks_null_sink_0, 0))    
        self.connect((self.doa_find_local_max_0, 1), (self.doa_compass, 0))    
        self.connect((self.phase_correct_hier_1, 0), (self.doa_autocorrelate_0, 0))    
        self.connect((self.phase_correct_hier_1, 1), (self.doa_autocorrelate_0, 1))    
        self.connect((self.phase_correct_hier_1, 2), (self.doa_autocorrelate_0, 2))    
        self.connect((self.phase_correct_hier_1, 3), (self.doa_autocorrelate_0, 3))    
        self.connect((self.twinrx_usrp_source_0, 0), (self.phase_correct_hier_1, 0))    
        self.connect((self.twinrx_usrp_source_0, 1), (self.phase_correct_hier_1, 1))    
        self.connect((self.twinrx_usrp_source_0, 2), (self.phase_correct_hier_1, 2))    
        self.connect((self.twinrx_usrp_source_0, 3), (self.phase_correct_hier_1, 3))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "run_MUSIC_lin_array_X310_TwinRX")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_input_variables(self):
        return self.input_variables

    def set_input_variables(self, input_variables):
        self.input_variables = input_variables

    def get_rel_phase_offsets_file_name(self):
        return self.rel_phase_offsets_file_name

    def set_rel_phase_offsets_file_name(self, rel_phase_offsets_file_name):
        self.rel_phase_offsets_file_name = rel_phase_offsets_file_name


def main(top_block_cls=run_MUSIC_lin_array_X310_TwinRX, options=None):

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
