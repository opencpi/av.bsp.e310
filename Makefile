$(if $(realpath $(OCPI_CDK_DIR)),,\
  $(error The OCPI_CDK_DIR environment variable is not set correctly.))
# This is the Makefile for the "e310" project.
include $(OCPI_CDK_DIR)/include/project.mk
