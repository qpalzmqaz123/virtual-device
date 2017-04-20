import sys
import re
from .enum2py import Enum2Py

# refert to vdev_conf.h : VDEV_SIMULATION_SOCKET_PATH
socket_file = '/tmp/vdev_socket'

# current python project root directory
root_dir = sys.path[0]

# virtual project root directory
vdev_root_dir = re.sub(r'/[^/]*($|(/$))', '', root_dir) + '/vdev'

# refer to vdev_model.h : vdev_model_t
model = Enum2Py('{}/inc/vdev_model.h'.format(vdev_root_dir), 'vdev_model_t')

# refer to vdev_types.h : vdev_status_t
status = Enum2Py('{}/inc/vdev_types.h'.format(vdev_root_dir), 'vdev_status_t')

