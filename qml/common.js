var theta_offset = 0;
var phi_offset = 0;
function setPhiOffset(tf) {
    phi_offset = tf
}
function setThetaOffset(tf) {
    theta_offset = tf
}
function subtraction(vm, vn) {
    return Qt.vector3d(vm.x - vn.x, vm.y - vn.y, vm.z - vn.z)
}

function length(vec) {
    return Math.sqrt((Math.pow(vec.x, 2) + Math.pow(vec.y, 2) + Math.pow(vec.z, 2)))
}

function dotProduct(vm, vn) {
    return (vm.x*vn.x + vm.y*vn.y + vm.z*vn.z)
}
function getTangentDirection(x0, y0, z0, R) {
    // 计算球面函数在球坐标系下的梯度
    var r = Math.sqrt(x0*x0 + y0*y0 + z0*z0); // 点 (x0, y0, z0) 到原点的距离
    var grad_r = { x: x0/r, y: y0/r, z: z0/r }; // r 方向的单位向量即为该点处的法向量

    // 将梯度转换到笛卡尔坐标系下，即为切线方向
    var grad_x = grad_r.x * x0 / r;
    var grad_y = grad_r.y * y0 / r;
    var grad_z = grad_r.z * z0 / r;

    // 返回切线方向的单位向量
    var length = Math.sqrt(grad_x * grad_x + grad_y * grad_y + grad_z * grad_z);
    return Qt.vector3d(-grad_x, -grad_y, -grad_z);
}
function getRotationMatrix(v1, v2) {
    // 计算旋转轴
    var rotationAxis = v1.crossProduct(v2);
    var rotationAngle = Math.acos(v1.dotProduct(v2) / (v1.length() * v2.length()));

    // 构建旋转矩阵
    var rotationMatrix = Qt.matrix4x4();
    rotationMatrix.rotate(rotationAngle * 180 / Math.PI, rotationAxis);

    return rotationMatrix;
}
function getZXYEulerAngles(rotationMatrix) {
    // 提取 ZXY 方向角
    var roll = Math.atan2(rotationMatrix.m32, rotationMatrix.m22);
    var pitch = Math.atan2(-rotationMatrix.m12, Math.sqrt(rotationMatrix.m32 * rotationMatrix.m32 + rotationMatrix.m22 * rotationMatrix.m22));
    var yaw = 0; // 在 ZXY 欧拉角中，yaw 角为 0

    // 将弧度转换为角度
    roll = roll * 180 / Math.PI + theta_offset;
    pitch = pitch * 180 / Math.PI + phi_offset;
    yaw = yaw * 180 / Math.PI;

    return Qt.vector3d(roll, pitch, yaw);
}
function getPose(origin, position, vector) {
    var dir = subtraction(position, origin);

    var dirlength = length(dir)

    if (dirlength === 0) {
        return Qt.vector3d(0, 0, 0);
    }
    // 计算模型的 z 轴向量
    var zAxis = Qt.vector3d(0, 0, 1);

    // 计算旋转矩阵，将模型的 z 轴旋转到法线向量的方向
    var rotationMatrix = getRotationMatrix(dir, zAxis);

    // 提取 ZXY 方向角
    var euler = getZXYEulerAngles(rotationMatrix);

    return euler;
}
function getRotationAngle(vec, R) {
    var x0 = vec.x;
    var y0 = vec.y;
    var z0 = vec.z;

    // 计算法线向量与 z 轴的夹角
    var cosTheta = z0 / Math.sqrt(x0 * x0 + y0 * y0 + z0 * z0);
    var theta = Math.acos(cosTheta);

    // 计算切平面与 x0y 平面的夹角
    var phi = Math.atan2(y0, x0);

    // 返回旋转角度
    return Qt.vector2d(theta, phi)
}

function angleToPosition(theta, phi, r) {

    var x = r * Math.sin(theta) * Math.cos(phi);
    var y = r * Math.sin(theta) * Math.sin(phi);
    var z = r * Math.cos(theta);

    return Qt.vector3d(x, y, z)
}
