from dumper import *
import gdb

def qdump__Bang__String(d, value):
    m_str = value["m_str"]
    d.putItem(m_str)
    d.putType("Bang::String")

def qdump__Bang__Path(d, value):
    m_absolutePath = value["m_absolutePath"]
    d.putItem(m_absolutePath)
    d.putType("Bang::Path")

def qdump__Bang__Array(d, value):
    m_vector = value["m_vector"]
    d.putItem(m_vector)
    d.putType("Bang::Array")

def qdump__Bang__List(d, value):
    m_list = value["m_list"]
    d.putItem(m_list)
    d.putType("Bang::List")

def qdump__Bang__Map(d, value):
    m_map = value["m_map"]
    d.putItem(m_map)
    d.putType("Bang::Map")

def qdump__Bang__UMap(d, value):
    m_umap = value["m_umap"]
    d.putItem(m_umap)
    d.putType("Bang::UMap")

def qdump__Bang__USet(d, value):
    m_uset = value["m_uset"]
    d.putItem(m_uset)
    d.putType("Bang::USet")

def qdump__Bang__Set(d, value):
    m_set = value["m_set"]
    d.putItem(m_set)
    d.putType("Bang::Set")

def qdump__Bang__Vector2G(d, v):
    xStr = "{:.2f}".format( float( str( v["x"].value() ) ) )
    yStr = "{:.2f}".format( float( str( v["y"].value() ) ) )

    vStr = "Vec3 (" + xStr + ", " + yStr + ")"
    d.putValue(vStr)

    d.putNumChild(2)
    if d.isExpanded():
        with Children(d):
            d.putSubItem("x", v["x"])
            d.putSubItem("y", v["y"])

def qdump__Bang__Vector3G(d, v):
    xStr = "{:.2f}".format( float( str( v["x"].value() ) ) )
    yStr = "{:.2f}".format( float( str( v["y"].value() ) ) )
    zStr = "{:.2f}".format( float( str( v["z"].value() ) ) )

    vStr = "Vec3 (" + xStr + ", " + yStr + ", " + zStr + ")"
    d.putValue(vStr)

    d.putNumChild(3)
    if d.isExpanded():
        with Children(d):
            d.putSubItem("x", v["x"])
            d.putSubItem("y", v["y"])
            d.putSubItem("z", v["z"])

def qdump__Bang__Vector4G(d, v):
    xStr = "{:.2f}".format( float( str( v["x"].value() ) ) )
    yStr = "{:.2f}".format( float( str( v["y"].value() ) ) )
    zStr = "{:.2f}".format( float( str( v["z"].value() ) ) )
    wStr = "{:.2f}".format( float( str( v["w"].value() ) ) )

    vStr = "Vec4 (" + xStr + ", " + yStr + ", " + zStr + ", " + wStr + ")"
    d.putValue(vStr)

    d.putNumChild(4)
    if d.isExpanded():
        with Children(d):
            d.putSubItem("x", v["x"])
            d.putSubItem("y", v["y"])
            d.putSubItem("z", v["z"])
            d.putSubItem("w", v["w"])

def qdump__Bang__QuaternionG(d, v):
    xStr = "{:.2f}".format( float( str( v["x"].value() ) ) )
    yStr = "{:.2f}".format( float( str( v["y"].value() ) ) )
    zStr = "{:.2f}".format( float( str( v["z"].value() ) ) )
    wStr = "{:.2f}".format( float( str( v["w"].value() ) ) )

    vStr = "Quat (" + xStr + ", " + yStr + ", " + zStr + ", " + wStr + ")"
    d.putValue(vStr)

    d.putNumChild(4)
    if d.isExpanded():
        with Children(d):
            d.putSubItem("x", v["x"])
            d.putSubItem("y", v["y"])
            d.putSubItem("z", v["z"])
            d.putSubItem("w", v["w"])

def qdump__Bang__Color(d, v):
    rStr = str( v["r"].value() )
    gStr = str( v["g"].value() )
    bStr = str( v["b"].value() )
    aStr = str( v["a"].value() )

    rStr = "{:.2f}".format( float(rStr) )
    gStr = "{:.2f}".format( float(gStr) )
    bStr = "{:.2f}".format( float(bStr) )
    aStr = "{:.2f}".format( float(aStr) )

    vStr = "Color (" + rStr + ", " + gStr + ", " + bStr + ", " + aStr + ")"
    d.putValue(vStr)

    d.putNumChild(4)
    if d.isExpanded():
        with Children(d):
            d.putSubItem("r", v["r"])
            d.putSubItem("g", v["g"])
            d.putSubItem("b", v["b"])
            d.putSubItem("a", v["a"])


