#!/usr/bin/python
"""
@brief Utility to convert rosbags into *.mat files

@author Matthew Powell
@author Ben Morris
@author Eric Cousineau
"""

import scipy.io # TODO Use HDF5?
import sys, os, collections
import rosbag
import string
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('filename', type=str)
parser.add_argument('-o', '--output', type=str, default=None)
args = parser.parse_args()

outputFile = args.output
if outputFile is None:
	base = os.path.splitext(args.filename)[0]
	outputFile = base + '.mat'

print 'Converting "{}" to "{}"'.format(args.filename, outputFile)
bag = rosbag.Bag(args.filename,'r')

pieces = []

def remove_leading_underscores_from_list(the_list):
	return [value for value in the_list if value.find('_') != 0]

def remove_blacklist_from_list(the_list,blacklist):
	return [value for value in the_list if not(value in blacklist)]

def append_value_to_structure(var,structure_depth_list,fieldname,value):
	tem = var
	for ii in range(len(structure_depth_list)):
		elem = structure_depth_list[ii]
		if not (elem in tem):
			tem[elem] = {}
		tem = tem[elem]	

	if not(fieldname in tem):
		tem[fieldname] = []
	
	return tem[fieldname].append(value)

def retrieve_element_of_structure(var,structure_depth_list):
	tem = var
	for ii in range(len(structure_depth_list)):
		elem = structure_depth_list[ii]
		if not (elem in tem):
			tem[elem] = {}
		tem = tem[elem]	

	return tem

def is_valid_type(value):
	types = [float, int, list, str]
	for cur in types:
		if isinstance(value, cur):
			return True
	return False

def is_number(s):
	try:
		float(s)
		return True
	except ValueError:
		return False
	except TypeError:
		return False

def safe_attribute(a, str_property):
	if hasattr(a, str_property):
		return getattr(a, str_property)
	else: 
		return False

def add_fields(var,topic,msg,plotflag,t=None):

	if safe_attribute(msg,'keys'):
		d = msg.keys()
	else: 
		d = dir(msg)

	blacklist = ['clear', 'itervalues', 'viewvalues', 'viewkeys', 'iterkeys', 'has_key', 'fromkeys', 'copy','iteritems','items', 'keys', 'update', 'values', 'viewitems', 'get','popitem', 'setdefault', 'is_integer', 'as_integer_ratio', 'fromhex', 'hex', 'conjugate', 'status', 'deserialize', 'deserialize_numpy', 'layout', 'serialize', 'serialize_numpy', 'header', 'reverse', 'sort', 'pop', 'count', 'insert', 'index', 'extend', 'append', 'remove', 'name', 'actualNames']
	fields = remove_leading_underscores_from_list(remove_blacklist_from_list(d,blacklist))

	if len(topic):
		topic_str = repr(topic)
		topic_str = topic_str.strip("'\/")
		structure_depth_list = topic_str.split("/")	
	else:
		structure_depth_list = {}

	if (plotflag > 0):
		print " "
		print "CALLED ADD_FIELDS(VAR,MSG,FIELDS)\n"
		print "VAR:" + repr(var) + '\n'
		print "MSG:" + '\n' + repr(msg) + '\n'
		print "FIELDS:" + repr(fields) + '\n'
		print "STRUCTURE_DEPTH_LIST:" + repr(structure_depth_list) + '\n'
		print "CONTINUING..."

	def add_time(element):
		global t
		if t is not None:
			t_mat = t.to_sec()
			field = 't_mat'
			if not field in element:
				element[field] = []
			element[field].append(t_mat)
			# Unset t so it is not repeatedly added
			t = None

	for field in fields:

		if isinstance(msg,dict):
			value = msg[field]
		else:
			value = getattr(msg, field)
		
		if not(isinstance(value, tuple) or isinstance(value,float)):

			if (safe_attribute(value,'_type') == "sensor_msgs/JointState"):

				element = retrieve_element_of_structure(var,structure_depth_list)
					
				tem={}
				tem["position"] = value.position
				tem["velocity"] = value.velocity
				tem["effort"] = value.effort

				add_fields(element,{},tem,0)
				add_time(element)

			else:

				if (safe_attribute(value,'len') > 0):
				
					for ii in range(len(value)):

						element = retrieve_element_of_structure(var,structure_depth_list)
						field_str = string.strip(repr(field),"'") + "_" + repr(ii)
						add_fields(element,field_str,value[ii],0)

				# else:
						# print "Encountered data type that is not a JointState or a float. Value =  " + repr(value) + "."
		else:

			element = retrieve_element_of_structure(var,structure_depth_list)

			if (plotflag > 0):
				print "ADDING A VALUE \n\n"
				print "STRUCTURE: " + repr(structure_depth_list)
				print "FIELD: " + repr(field)
				print "VALUE: " + repr(value) + "\n"

			if not(field in element):
				element[field] = []

			element[field].append(value)
			add_time(element)

rosbag_data={}
for topic, msg, t in bag.read_messages():
	add_fields(rosbag_data,topic,msg,0,t)

absPath = os.path.abspath(outputFile)
scipy.io.savemat(outputFile, {'rosbag': rosbag_data}, oned_as='column')
print "Done"