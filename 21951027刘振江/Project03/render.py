import numpy as np
import cv2
import os
from PIL import Image
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.osmesa import *

os.environ["PYOPENGL_PLATFORM"] = "osmesa"


def get_texture_image(filename):
    img = cv2.imread(filename)
    img = img[:, :, [2, 1, 0]]
    img = np.flip(img, axis=0)
    texture_data = bytearray(img)
    height, width = img.shape[0], img.shape[1]
    return height, width, texture_data


def obj_loader(model_path):
    lines = open(model_path).readlines()
    lines = [line.strip().split() for line in lines]
    lines = [line for line in lines if len(line) != 0]

    vertex = [line[1:] for line in lines if line[0] == 'v']

    for x in vertex:
        if len(x) != 3:
            print(x)
    vertex = np.array(vertex)
    vertex = vertex.astype(np.float32)

    xmin, xmax = vertex[:, 0].min(), vertex[:, 0].max()
    ymin, ymax = vertex[:, 1].min(), vertex[:, 1].max()
    zmin, zmax = vertex[:, 2].min(), vertex[:, 2].max()
    vertex[:, 0] -= (xmax + xmin) / 2.0
    vertex[:, 1] -= (ymax + ymin) / 2.0
    vertex[:, 2] -= (zmax + zmin) / 2.0
    scale = max(max(xmax - xmin, ymax - ymin), zmax - zmin) / 2.0
    vertex /= scale
    num_vertex = vertex.shape[0]

    uv = [line[1:3] for line in lines if line[0] == 'vt']
    uv = np.array(uv)
    uv = uv.astype(np.float32)
    num_uv = uv.shape[0]

    face_all = [line[1:] for line in lines if line[0] == 'f']

    face = [[int(y.split('/')[0]) for y in x] for x in face_all]
    face_texture = [[int(y.split('/')[1]) for y in x] for x in face_all]

    def face_index(num, f):
        if f > 0:
            return f - 1
        if f < 0:
            return num + f

    tri_face = []
    for f in face:
        if len(f) == 4:
            tri_face.append(face_index(num_vertex, f[0]))
            tri_face.append(face_index(num_vertex, f[1]))
            tri_face.append(face_index(num_vertex, f[2]))

            tri_face.append(face_index(num_vertex, f[2]))
            tri_face.append(face_index(num_vertex, f[3]))
            tri_face.append(face_index(num_vertex, f[0]))

        if len(f) == 3:
            tri_face.append(face_index(num_vertex, f[0]))
            tri_face.append(face_index(num_vertex, f[1]))
            tri_face.append(face_index(num_vertex, f[2]))

    tri_face_uv = []
    for f in face_texture:
        if len(f) == 4:
            tri_face_uv.append(face_index(num_uv, f[0]))
            tri_face_uv.append(face_index(num_uv, f[1]))
            tri_face_uv.append(face_index(num_uv, f[2]))

            tri_face_uv.append(face_index(num_uv, f[2]))
            tri_face_uv.append(face_index(num_uv, f[3]))
            tri_face_uv.append(face_index(num_uv, f[0]))

        if len(f) == 3:
            tri_face_uv.append(face_index(num_uv, f[0]))
            tri_face_uv.append(face_index(num_uv, f[1]))
            tri_face_uv.append(face_index(num_uv, f[2]))

    vertex = vertex[tri_face, :]
    uv = uv[tri_face_uv, :]

    return vertex, uv


def osmesa_init(height, width):
    PyMem_Malloc = ctypes.pythonapi.PyMem_Malloc
    PyMem_Malloc.restype = ctypes.c_void_p
    PyMem_Malloc.argtypes = [ctypes.c_size_t]

    ctx = OSMesaCreateContextExt(OSMESA_RGB, 16, 0, 0, None)
    buffer = PyMem_Malloc(width * height * 3)
    OSMesaMakeCurrent(ctx, ctypes.cast(buffer, ctypes.c_void_p), GL_UNSIGNED_BYTE, width, height)
    return ctx, buffer


def osmesa_finish(ctx, buffer):
    OSMesaDestroyContext(ctx)
    PyMem_Free = ctypes.pythonapi.PyMem_Free
    PyMem_Free.restype = None
    PyMem_Free.argtypes = [ctypes.c_void_p]
    PyMem_Free(buffer)


def render_color(vertex, uv, texture_height, texture_width, texture_data):
    glEnable(GL_DEPTH_TEST)

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data)
    glEnable(GL_TEXTURE_2D)

    glClearDepth(1)
    glClearColor(0.0, 0.0, 0.0, 1.0)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    glBegin(GL_TRIANGLES)
    glColor(1.0, 1.0, 1.0)
    for i in range(vertex.shape[0]):
        glTexCoord2f(uv[i, 0], uv[i, 1])
        glVertex3f(vertex[i, 0], vertex[i, 1], vertex[i, 2])
    glEnd()
    glFlush()


def world_to_ndc_R(vertex, height, width, R, tx, ty):
    V = vertex.transpose()
    rvt = np.matmul(R, V)

    ndc = rvt
    z_max, z_min = ndc[2, :].max(), ndc[2, :].min()

    ndc[2, :] -= z_max
    ndc[2, :] /= -(z_max - z_min)

    ndc[0, :] += tx
    ndc[1, :] += ty

    window_ratio = width / height
    ndc[1, :] *= window_ratio

    ndc = ndc.transpose()
    return ndc, z_max, z_min


def get_render_R(vertex, texture_uv, texture_data, texture_height, texture_width, height, width, R, tx, ty,):
    ndc, z_max, z_min = world_to_ndc_R(vertex, height, width, R, tx, ty)

    ctx, buffer = osmesa_init(height, width)
    render_color(ndc, texture_uv, texture_height, texture_width, texture_data)
    ctypes_array = (ctypes.c_char * (width * height * 3)).from_address(buffer)
    render_result = np.frombuffer(ctypes_array, dtype=np.uint8)
    render_result = render_result.copy()
    render_result = render_result.reshape([height, width, 3])

    depth_render = glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT)
    depth_render = depth_render.copy()
    depth_render = depth_render.reshape(height, width)
    mask = depth_render != 1
    mask = mask.astype(np.uint8)

    osmesa_finish(ctx, buffer)

    return render_result, mask


def process():
    height, width = 480, 640
    scale = 0.2688189744949341
    R = np.eye(3) * 1.1 * scale
    tx, ty = [0.26850757, 0.38995805]

    v, uv = obj_loader('model.obj')
    texture_height, texture_width, texture_data = get_texture_image(
        'material.jpg')

    render_result, mask = get_render_R(v, uv,
                                     texture_data, texture_height, texture_width,
                                     height, width, R, tx, ty)

    cv2.imshow('render_result', render_result)
    cv2.waitKey()


if __name__ == '__main__':
    process()
