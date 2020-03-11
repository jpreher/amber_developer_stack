#!/usr/bin/env python
"""
@brief Utility to enumerate available fields for all available topics

@author Paul Dinh <paul.dinh@nasa.gov>

Contributions from Barrett Ames <cbames@gmail.com>
"""
from PyQt4 import QtGui, QtCore

import sys, signal
from pprint import pprint
import subprocess

import rospy
# import roslib; roslib.load_manifest('plotter')
from std_msgs.msg import *
from geometry_msgs.msg import *
from sensor_msgs.msg import *
from nav_msgs.msg import *
import time
import signal
from functools import partial

class TopicListener(object):
    def setupSubscribers(self):
        """Query roscore for supported topics (only supports Float64MultiArray right now) and creates subscriber to get dimension."""
        topics = rospy.get_published_topics()
        supported_types = ['std_msgs/Float32MultiArray',
                           'std_msgs/Float64MultiArray',
                           'std_msgs/Int16MultiArray',
                           'std_msgs/Int32MultiArray',
                           'std_msgs/Int64MultiArray',
                           'std_msgs/UInt8MultiArray',
                           'std_msgs/UInt16MultiArray',
                           'std_msgs/UInt32MultiArray',
                           'std_msgs/UInt64MultiArray',
                           'std_msgs/Float32',
                           'std_msgs/Float64',
                           'std_msgs/Int16',
                           'std_msgs/Int32',
                           'std_msgs/Int64',
                           'std_msgs/UInt16',
                           'std_msgs/UInt32',
                           'std_msgs/UInt64',
                           'geometry_msgs/WrenchStamped',
                           'geometry_msgs/TwistStamped',
                           'geometry_msgs/PointStamped',
                           'geometry_msgs/PoseStamped',
                           'geometry_msgs/Pose',
                           'nav_msgs/Odometry',
                           'sensor_msgs/JointState']
        valid_topics = {}
        for topic_name, topic_type in topics:
            if topic_type in supported_types:
                if topic_type not in valid_topics:
                    valid_topics[topic_type] = {}
                valid_topics[topic_type][topic_name] = {}

                # sorry whoever is reading this
                def setPose(tt, tn, data):
                    valid_topics[tt][tn]['size']['position/x'] = 1
                    valid_topics[tt][tn]['size']['position/y'] = 1
                    valid_topics[tt][tn]['size']['position/z'] = 1
                    valid_topics[tt][tn]['size']['orientation/x'] = 1
                    valid_topics[tt][tn]['size']['orientation/y'] = 1
                    valid_topics[tt][tn]['size']['orientation/z'] = 1
                    valid_topics[tt][tn]['size']['orientation/w'] = 1

                def setPoseStamped(tt, tn, data):
                    valid_topics[tt][tn]['size']['pose/position/x'] = 1
                    valid_topics[tt][tn]['size']['pose/position/y'] = 1
                    valid_topics[tt][tn]['size']['pose/position/z'] = 1
                    valid_topics[tt][tn]['size']['pose/orientation/x'] = 1
                    valid_topics[tt][tn]['size']['pose/orientation/y'] = 1
                    valid_topics[tt][tn]['size']['pose/orientation/z'] = 1
                    valid_topics[tt][tn]['size']['pose/orientation/w'] = 1

                def setPointStamped(tt, tn, data):
                    valid_topics[tt][tn]['size']['point/x'] = 1
                    valid_topics[tt][tn]['size']['point/y'] = 1
                    valid_topics[tt][tn]['size']['point/z'] = 1

                def setOdometry(tt, tn, data):
                     valid_topics[tt][tn]['size']['pose/pose/position/x'] = 1
                     valid_topics[tt][tn]['size']['pose/pose/position/y'] = 1
                     valid_topics[tt][tn]['size']['pose/pose/position/z'] = 1
                     valid_topics[tt][tn]['size']['pose/pose/orientation/x'] = 1
                     valid_topics[tt][tn]['size']['pose/pose/orientation/y'] = 1
                     valid_topics[tt][tn]['size']['pose/pose/orientation/z'] = 1
                     valid_topics[tt][tn]['size']['pose/pose/orientation/w'] = 1

                def setWrenchStamped(tt, tn, data):
                     valid_topics[tt][tn]['size']['wrench/force/x'] = 1
                     valid_topics[tt][tn]['size']['wrench/force/y'] = 1
                     valid_topics[tt][tn]['size']['wrench/force/z'] = 1
                     valid_topics[tt][tn]['size']['wrench/torque/x'] = 1
                     valid_topics[tt][tn]['size']['wrench/torque/y'] = 1
                     valid_topics[tt][tn]['size']['wrench/torque/z'] = 1

                def setTwistStamped(tt, tn, data):
                     valid_topics[tt][tn]['size']['twist/linear/x'] = 1
                     valid_topics[tt][tn]['size']['twist/linear/y'] = 1
                     valid_topics[tt][tn]['size']['twist/linear/z'] = 1
                     valid_topics[tt][tn]['size']['twist/angular/x'] = 1
                     valid_topics[tt][tn]['size']['twist/angular/y'] = 1
                     valid_topics[tt][tn]['size']['twist/angular/z'] = 1

                def setJoint(tt, tn, data):
                    valid_topics[tt][tn]['size'] = {'position': len(data.position), 'velocity': len(data.velocity), 'effort': len(data.effort), 'name': data.name}

                # sorry whoever is reading this
                pose = lambda d, tt=topic_type, tn=topic_name: setPose(tt,tn,d)
                posestamped = lambda d, tt=topic_type, tn=topic_name: setPoseStamped(tt,tn,d)
                pointstamped = lambda d, tt=topic_type, tn=topic_name: setPointStamped(tt,tn,d)
                twiststamped = lambda d, tt=topic_type, tn=topic_name: setTwistStamped(tt,tn,d)
                wrenchstamped = lambda d, tt=topic_type, tn=topic_name: setWrenchStamped(tt,tn,d)
                odom = lambda d, tt=topic_type, tn=topic_name: setOdometry(tt,tn,d)
                joint = lambda d, tt=topic_type, tn=topic_name: setJoint(tt,tn,d)
                array = lambda d, tt=topic_type, tn=topic_name: valid_topics[tt][tn]['size'].__setitem__('data', d.layout.dim[0].size)
                single = lambda d, tt=topic_type, tn=topic_name: valid_topics[tt][tn]['size'].__setitem__('data', 1)
                a = partial(array)
                s = partial(single)
                p = partial(pose)
                ps = partial(posestamped)
                points = partial(pointstamped)
                ts = partial(twiststamped)
                ws = partial(wrenchstamped)
                j = partial(joint)
                odom = partial(odom)

                if 'MultiArray' in topic_type:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), a)
                elif 'PoseStamped' in topic_type:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), ps)
                elif 'PointStamped' in topic_type:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), points)
                elif 'TwistStamped' in topic_type:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), ts)
                elif 'WrenchStamped' in topic_type:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), ws)
                elif 'Odometry' in topic_type:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), odom)
                elif 'Pose' in topic_type:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), p)
                elif 'JointState' in topic_type:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), j)
                else:
                    valid_topics[topic_type][topic_name]['subscriber'] = rospy.Subscriber(topic_name, eval(topic_type.rsplit('/', 1)[-1]), s)
                valid_topics[topic_type][topic_name]['size'] = {}

        return valid_topics

    def getTopicData(self, t):
        """Listen to topics for t seconds to get data (length, etc) and unsubscribe."""
        if type(t) != int:
            return
        valid_topics = self.setupSubscribers()
        for x in range(t):
            time.sleep(1.0)
        for topic_type, val in valid_topics.iteritems():
            for topic_name, stuff in val.iteritems():
                valid_topics[topic_type][topic_name]['subscriber'].unregister()

        return valid_topics

class TopicItem(QtGui.QStandardItem):
    ROLEPARENT = 32 # True if parent (check/uncheck all children if checked/unchecked and don't plot this directly)
    ROLEINDEX = 33  # we're mapping joint names in the list, this will track the original index in the array
    def __init__(self, key):
        super(TopicItem, self).__init__(key)
        self.setForeground(QtCore.Qt.black)
        self.setCheckable(True)
        self.setEditable(False)
        self.setData(False, self.ROLEPARENT)
        self.setData(-1, self.ROLEINDEX)

    @property
    def parent(self):
        """Return True if item is a parent row."""
        return bool(self.data(self.ROLEPARENT).toPyObject())

    @parent.setter
    def parent(self, value):
        self.setData(value, self.ROLEPARENT)

    @property
    def index(self):
        """Return the original index in the array."""
        return self.data(self.ROLEINDEX).toPyObject()

    @index.setter
    def index(self, value):
        self.setData(value, self.ROLEINDEX)

class Plotter(QtGui.QWidget):
    def __init__(self, debug):
        super(Plotter, self).__init__()
        self.debug = debug
        self.topic_listener = TopicListener()
        signal.signal(signal.SIGINT, signal.SIG_DFL)
        self.setupWidgets()
        self.valid_topics = {}
        self.refreshCb()
        self.show()

    def setupWidgets(self):
        self.setLayout(QtGui.QVBoxLayout())
        self.layout().setAlignment(QtCore.Qt.AlignTop)

        self.treeView = QtGui.QTreeView()
        self.model = QtGui.QStandardItemModel()
        self.model.itemChanged.connect(self.itemChanged)
        self.treeView.setModel(self.model)

        refresh = QtGui.QPushButton("Refresh")
        refresh.clicked.connect(self.refreshCb)
        collapse = QtGui.QPushButton("Collapse")
        collapse.clicked.connect(self.treeView.collapseAll)
        expand = QtGui.QPushButton("Expand")
        expand.clicked.connect(self.treeView.expandAll)
        hbox = QtGui.QHBoxLayout()
        hbox.addWidget(collapse)
        hbox.addWidget(expand)

        plot = QtGui.QPushButton("Plot Checked")
        plot.clicked.connect(self.plotCb)

        self.layout().addWidget(refresh)
        self.layout().addLayout(hbox)
        self.layout().addWidget(plot)
        self.layout().addWidget(self.treeView)
        self.resize(500,800)

    def itemChanged(self, item):
        """Check item callback."""
        # if parent, check/uncheck all children
        if item.parent:
            children = []
            if item.hasChildren():
                for i in range(item.rowCount()):
                    item.child(i).setCheckState(item.checkState())

    def plotCb(self):
        """Plot checked topics. User is responsible for closing the opened plot."""
        topics = []
        for i in range(self.model.rowCount()):
            parentItem = self.model.item(i)
            if parentItem.hasChildren():
                for j in range(parentItem.rowCount()):
                    childItem = parentItem.child(j)
                    if childItem.checkState() == QtCore.Qt.Checked:
                        if childItem.index == -1:
                            name = ''.join([str(parentItem.text()), str(childItem.text())])
                        else:
                            topic = str(childItem.text())
                            joint_name = topic[topic.find("[")+1:topic.find("]")]
                            topic = topic.replace(joint_name, str(childItem.index))
                            name = ''.join([str(parentItem.text()), topic])
                        topics.append(name)

        # call rqt_plot with checked topics, using str.join to build the command line argument with comma as a separator
        # print ','.join(topics)
        args = ['rqt_plot', ','.join(topics)]
        subprocess.Popen(args)

    def refreshCb(self):
        """"Refresh topics and add them to the TreeView."""
        self.valid_topics = self.topic_listener.getTopicData(1)
        self.model.clear()
        self.addTopics()

    def addTopics(self):
        """Add topics to TreeView."""
        for topic_type in self.valid_topics.keys():
            for topic_name, stuff in self.valid_topics[topic_type].iteritems():
                if stuff['size'] == {}:
                    # ignore topics with no data
                    continue
                item = TopicItem(topic_name)
                item.parent = True

                # special case for joint_states
                if topic_type == 'sensor_msgs/JointState':
                    for key, length in stuff['size'].iteritems():
                        if key != 'name':
                            for i in range(length):
                                item2 = TopicItem("/" + key + "[" + str(i) + ' - ' + stuff['size']['name'][i] + "]")
                                item2.index = i
                                item.appendRow([item2])
                else:
                    for key, length in stuff['size'].iteritems():
                        if length == 1:
                            item2 = TopicItem("/" + key)
                            item.appendRow([item2])
                        else:
                            for i in range(length):
                                item2 = TopicItem("/" + key + "[" + str(i) + "]")
                                item.appendRow([item2])
                self.model.appendRow([item])
        self.model.sort(0)

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    rospy.init_node('TopicListener', anonymous=True)
    # start the application
    debug = False
    if len(sys.argv) > 1:
        if sys.argv[1] == '-d':
            debug = True
    ex = Plotter(debug)
    sys.exit(app.exec_())
