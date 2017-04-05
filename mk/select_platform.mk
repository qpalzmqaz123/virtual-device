# default platform is posix, the platform.mk must be exist in mk folder
ifndef PLATFORM
PLATFORM = posix
endif

# select the platform
platform_file = mk/$(PLATFORM).mk
include $(platform_file)
