import math
import sys

from geometry_msgs.msg import TransformStamped

import numpy as np

import rclpy
from rclpy.node import Node

from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster


def quaternion_from_euler_zyx(ai, aj, ak):
    ai /= 2.0
    aj /= 2.0
    ak /= 2.0
    ci = math.cos(ai)
    si = math.sin(ai)
    cj = math.cos(aj)
    sj = math.sin(aj)
    ck = math.cos(ak)
    sk = math.sin(ak)
    cc = ci*ck
    cs = ci*sk
    sc = si*ck
    ss = si*sk

    q = np.empty((4, ))
    q[0] = cj*sc - sj*cs
    q[1] = cj*ss + sj*cc
    q[2] = cj*cs - sj*sc
    q[3] = cj*cc + sj*ss

    return q


class StaticFramePublisher(Node):

    def __init__(self, n):
        super().__init__('my_static_cart_tf2_broadcaster')

        self.tf_static_broadcaster = StaticTransformBroadcaster(self)

        # for i in range(n):
        # Publish static transforms once at startup
        """
        self.transformations = [
            ['cart_base', 'marker_0', 0      , -0.06 , 0.41, math.radians(99) , 0 , 0                ],
            ['cart_base', 'marker_1', 0.215  , 0.362 , 0.41, math.radians(92) , 0 , math.radians(90) ],
            ['cart_base', 'marker_2', 0      , 0.81  , 0.41, math.radians(106), 0 , math.radians(180)],
            ['cart_base', 'marker_3', -0.215 , 0.365 , 0.41, math.radians(92) , 0 , math.radians(-90)]
        ]
        """

        self.transformations = [
            ['marker_0', 'cart_base_0', 0      , -0.418 , 0.01  , 0                 , math.radians(99) , math.radians(90) ],
            ['marker_1', 'cart_base_1', -0.362 , -0.410 , -0.2  , 0                 , math.radians(-92) , math.radians(-90) ],
            ['marker_2', 'cart_base_2', 0      , -0.615 , -0.664, math.radians(106) , 0                 , math.radians(-180)],
            ['marker_3', 'cart_base_3', 0.362  , -0.410 , -0.2  , 0                 , math.radians(92)  , math.radians(90)  ]
        ]

        self.make_transforms()


    def make_transforms(self):

        transforms = []

        for transfomation in self.transformations:
            t = TransformStamped()

            t.header.stamp = self.get_clock().now().to_msg()
            t.header.frame_id = transfomation[0]
            t.child_frame_id = transfomation[1]

            t.transform.translation.x = float(transfomation[2])
            t.transform.translation.y = float(transfomation[3])
            t.transform.translation.z = float(transfomation[4])
            quat = quaternion_from_euler_zyx(
                float(transfomation[5]), float(transfomation[6]), float(transfomation[7]))
            t.transform.rotation.x = quat[0]
            t.transform.rotation.y = quat[1]
            t.transform.rotation.z = quat[2]
            t.transform.rotation.w = quat[3]

            transforms.append(t)

        self.tf_static_broadcaster.sendTransform(transforms)


def main():
    logger = rclpy.logging.get_logger('logger')

    # obtain parameters from command line arguments
    if len(sys.argv) != 1:
        logger.info('Invalid number of parameters. Usage: \n'
                    '$ ros2 run my_tf2_package my_static_cart_tf2_broadcaster'
                    'n')
        sys.exit(1)

    if sys.argv[0] == '0':
        logger.info('cart number cannot be "0"')
        sys.exit(2)

    # pass parameters and initialize node
    rclpy.init()
    node = StaticFramePublisher(sys.argv)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    rclpy.shutdown()

if __name__ == '__main__':
    main()

