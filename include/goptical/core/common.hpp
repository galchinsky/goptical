/*

      This file is part of the <goptical/core Core library.
  
      The <goptical/core library is free software; you can redistribute it
      and/or modify it under the terms of the GNU General Public
      License as published by the Free Software Foundation; either
      version 3 of the License, or (at your option) any later version.
  
      The <goptical/core library is distributed in the hope that it will be
      useful, but WITHOUT ANY WARRANTY; without even the implied
      warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
      See the GNU General Public License for more details.
  
      You should have received a copy of the GNU General Public
      License along with the <goptical/core library; if not, write to the
      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
      Boston, MA 02111-1307 USA
  
      Copyright (C) 2010-2011 Free Software Foundation, Inc
      Author: Alexandre Becoulet

*/

#ifdef _goptical
# error
#endif

#ifndef GOPTICAL_COMMON_HH_
#define GOPTICAL_COMMON_HH_

#ifdef HAVE_CONFIG_H
# include <config.hpp>
#endif

#include <list>
#include <map>
#include <vector>
#include <deque>
#include <functional>

#include "goptical/core/vector_pool" /* mkdoc:skip */
#include "goptical/core/ref"    /* mkdoc:skip */
#include "goptical/core/fstring"        /* mkdoc:skip */
#include "goptical/core/vlarray"        /* mkdoc:skip */

#include <math.h>

#define PATH_SEPARATOR '/'

#define GOPTICAL_REFNEW(...) DPP_REFNEW(__VA_ARGS__)

#ifdef CONFIG_GOPTICAL_DEBUG
# define GOPTICAL_DEBUG(s) std::cerr << s << std::endl
#else
# define GOPTICAL_DEBUG(s)
#endif

  /** @internal @showcontent
      Provide bit mask operations for an enum type */
#define GOPTICAL_MASK_ENUM_OPS(type)                            \
  /** Bitwise or operation for use on @ref type enum. */        \
  inline type operator| (type a, type b)                        \
  {                                                             \
    return (type)((int)a | (int)b);                             \
  }                                                             \
  /** Bitwise and operation for use on @ref type enum. */       \
  inline type operator& (type a, type b)                        \
  {                                                             \
    return (type)((int)a & (int)b);                             \
  }                                                             \
  /** Bitwise not operation for use on @ref type enum. */       \
  inline type operator~ (type a)                                \
  {                                                             \
    return (type)(~(int)a);                                     \
  }

  /** @internal @showcontent */
#define GOPTICAL_SET_ACCESSOR(type, name, help) \
  /** @see get_##name Set ""help"". */          \
  inline void set_##name(type name)             \
  {                                             \
    _##name = name;                             \
  }

  /** @internal @showcontent */
#define GOPTICAL_GET_ACCESSOR(type, name, help) \
  /** @see set_##name Get ""help"". */          \
  inline type get_##name() const                \
  {                                             \
    return _##name;                             \
  }

  /** @internal @showcontent */
#define GOPTICAL_ACCESSORS(type, name, help)    \
  GOPTICAL_GET_ACCESSOR(type, name, help)       \
  GOPTICAL_SET_ACCESSOR(type, name, help)

  /** @internal @showcontent */
#define GOPTICAL_GET_REF_ACCESSOR(type, name, help)     \
  /** @see set_##name Get ""help"". */                  \
  inline const type & get_##name() const                \
  {                                                     \
    return _##name;                                     \
  }

  /** @internal @showcontent */
#define GOPTICAL_GET_NOCONST_REF_ACCESSOR(type, name, help)     \
  /** @see set_##name Get ""help"". */                          \
  inline type & get_##name()                                    \
  {                                                             \
    return _##name;                                             \
  }

  /** @internal @showcontent */
#define GOPTICAL_SET_REF_ACCESSOR(type, name, help)     \
  /** @see get_##name Set ""help"". */                  \
  inline void set_##name(const type &name)              \
  {                                                     \
    _##name = name;                                     \
  }

  /** @internal @showcontent */
#define GOPTICAL_REF_ACCESSORS(type, name, help)        \
  GOPTICAL_GET_REF_ACCESSOR(type, name, help)           \
  GOPTICAL_SET_REF_ACCESSOR(type, name, help)

  /** @internal @showcontent */
#define GOPTICAL_NOCONST_REF_ACCESSORS(type, name, help)        \
  GOPTICAL_GET_REF_ACCESSOR(type, name, help)                   \
  GOPTICAL_GET_NOCONST_REF_ACCESSOR(type, name, help)           \
  GOPTICAL_SET_REF_ACCESSOR(type, name, help)

  /** @internal @showcontent */
#define GOPTICAL_GET_NOCONST_REF_ACCESSORS(type, name, help)    \
  GOPTICAL_GET_REF_ACCESSOR(type, name, help)                   \
  GOPTICAL_GET_NOCONST_REF_ACCESSOR(type, name, help)

// enums can not be forward declared, that's why they need to live in
// the <goptical/core namespace
namespace goptical {
  using namespace dpp;

  // enums
  namespace sys {

    enum LensEdge
      {
        StraightEdge,
        SlopeEdge,
      };

    /** Specifies point source location mode */
    enum SourceInfinityMode
      {
        /** In finite distance mode the point source is located at
            specified position and all rays are traced from this
            point. */
        SourceAtFiniteDistance,
        /** In infinity mode the point source generate parallel rays
            oriented along source direction vector. */
        SourceAtInfinity,
      };

  }

  namespace trace {

    /** Specifies point distribution patterns over a shape delimited surface.
        @image dist_patterns.png {Different patterns rendered on a disk with default density}
        @see Distribution
    */
    enum Pattern
      {
        /** Preferred distribution pattern for a given shape */
        DefaultDist,
        /** Sagittal plane distribution (along the X axis, X/Z plane) */
        SagittalDist,
        /** Meridional plane distribution (along the Y axis, Y/Z plane) */
        MeridionalDist,
        /** Tangential plane distribution, same as @ref MeridionalDist */
        TangentialDist = MeridionalDist,
        /** Sagittal and Meridional distribution combined */
        CrossDist,
        /** Square pattern distribution */
        SquareDist,
        /** Triangular pattern distribution */
        TriangularDist,
        /** Hexapolar pattern, suitable for circular shapes */
        HexaPolarDist,
        /** Random distribution */
        RandomDist
      };

    /** Specifies light intensity calculation mode to use by light propagation algorithms. */
    enum IntensityMode
      {
        /** No Intensity calculation is performed */
        Simpletrace, 
        /** light intensity computation is performed without taking polarization into account */
        Intensitytrace,
        /** @experimental light intensity with polarization computation is performed (not supported yet) */
        Polarizedtrace
      };

    /** @experimental @hidden 
        Specifies physical light propagation algorithm/model */
    enum PropagationMode
      {
        /** Use classical ray tracing algorithm to propagate light. */
        RayPropagation,
        /** Use Diffraction based light propagation */
        DiffractPropagation,
        /** Used mixed ray tracing/diffraction propagation */
        MixedPropagation
      };

  }

  namespace material {

    /** Specify air refractive index model used by @ref Air class. */
    enum AirFormula
      {
        AirBirch94Formula,             //< Birch, Metrologia, 1994, 31, 315
        AirKohlrausch68Formula,        //< F. Kohlrausch, Praktische Physik, 1968, 1, 408
      };

    /** Specify Sellmeier variation used by the @ref SellmeierMod class */
    enum SellmeierModFormula
      {
        SellmeierMod2Formula,
        Handbook1Formula,
        Handbook2Formula
      };

    /** Specify Abbe number type used by the @ref Abbe class */
    enum AbbeFormula
      {
        AbbeVdFormula,    //< Abbe number of Fraunhofer @em d line
        AbbeVeFormula,    //< Abbe number of Fraunhofer @em e line
      };

  }

  namespace io {

    /** Specifies rendered text alignment */
    enum TextAlignMask
      {
        TextAlignCenter = 1,   //< Vertically centered
        TextAlignLeft = 2,
        TextAlignRight = 4,
        TextAlignTop = 8,
        TextAlignBottom = 16,
        TextAlignMiddle = 32,   //< Horizontally centered
      };

    GOPTICAL_MASK_ENUM_OPS(TextAlignMask);

    /** Specifies light ray intensity rendering mode */
    enum IntensityMode
      {
        /** light ray intensity is ignored, no blending is performed while rendering ray */
        IntensityIgnore,
        /** light ray intensity is used to blend rendered ray */
        IntensityShade,
        /** light ray intensity logarithm is used to blend rendered ray. This enable faint rays to remain visible. */
        IntensityLogShade
      };

    /** Specifies light ray color rendering */
    enum RayColorMode
      {
        /** Compute ray color from its wavelength */
        RayColorWavelen,
        /** Use fixed ray color */
        RayColorFixed
      };

    /** Specifies rendering elements which can have modified colors and style */
    enum Style
      {
        StyleBackground,
        StyleForeground,
        StyleRay,
        StyleSurface,
        StyleGlass,
        StyleLast
      };

    enum PointStyle
      {
        PointStyleDot,
        PointStyleCross,
        PointStyleRound,
        PointStyleSquare,
        PointStyleTriangle,
      };

  }

  namespace data {

    /** Specifies available styles for 2d and 3d plot data rendering */
    enum PlotStyleMask
      {
        /** Draw lines between knots */
        LinePlot = 1,
        /** Draw points for each knot */
        PointPlot = 2,
        /** Draw a smooth curve */
        InterpolatePlot = 4,
        /** Print ploted values near knots */
        ValuePlot = 8,
        /** Draw filled 3d surface */
        Filled = 16,
      };

    GOPTICAL_MASK_ENUM_OPS(PlotStyleMask);

    /** Specifies data interpolation methods. Availability depends on data container used. */
    enum Interpolation
      {
        /** 1d and 2d nearest interpolation */
        Nearest,

        /** 1d linear and 2d bilinear interpolations */
        Linear,

        /** 1d quadratic interpolation */
        Quadratic,

        /** 1d cubic piecewise interpolation. It has continuous
            piecewise first derivative, non-continuous piecewise
            linear second derivative. Use segments slope as first
            derivative. Less accurate than other cubic interpolants
            but requires less computation on data set change. */
        CubicSimple,

        /** 1d cubic piecewise interpolation. It has smooth first
            derivative and continuous piecewise linear second
            derivative. Derivatives for first and last entries are
            based on first and last segments slope. It uses linear
            extrapolation (continuous but non-smooth first derivative
            on both ends). */
        Cubic,

        /** Same interpolation as Cubic, with quadratic extrapolation
            (continous and smooth first derivative on both ends). */
        Cubic2,

        /** Same as Cubic with first derivative prescribed for first
            and last entries only. */
        CubicDerivInit,

        /** Same as Cubic2 with first derivative prescribed for first
            and last entries only. */
        Cubic2DerivInit,

        /** 1d cubic piecewise interpolation. First derivatives must
            be provided for all entries. It uses linear extrapolation.*/
        CubicDeriv,

        /** 1d cubic piecewise interpolation. First derivatives must
            be provided for all entries. It uses quadratic extrapolation.*/
        Cubic2Deriv,

        /** 2d bicubic interpolation. Use smooth first derivative and
            continuous piecewise linear second derivative. Use 1d
            cubic curve to extract gradients (smooth first derivative
            and continuous piecewise linear second derivative). This
            is the best 2d interpolation when derivatives are
            non-prescribed. */
        Bicubic,

        /** 2d bicubic interpolation. Use numerical differencing to
            extract gradients. Less accurate than @ref Bicubic but
            requires less computation on data set change.*/
        BicubicDiff,

        /** 2d bicubic interpolation. x and y gradients must be
            provided. This is the best 2d interpolation when
            derivatives values are available. */
        BicubicDeriv,
      };
  }

}

// We use the _goptical namespace internally in .hpp and .hxx files and
// only copy declarations to the <goptical/core namespace when the associated
// UpperCase header file is included by user.

#ifdef __MKDOC__
# define _goptical <goptical/core
#endif

/** @short <goptical/core library namespace */
namespace _goptical {

  using namespace dpp;

  /** @module {Core}
      @short mathematical tools and functions */
  namespace math
  {
    template <int N, typename T = double> struct Vector;
    template <typename T> struct Vector<3, T>;
    template <typename T> struct Vector<2, T>;
    /** @main @see __Vector2__ */
    typedef Vector<2, double> Vector2;
    /** @main @see __Vector3__ */
    typedef Vector<3, double> Vector3;

    template <int N> struct VectorPair;
    /** @see __VectorPair2__ */
    typedef VectorPair<2> VectorPair2;
    /** @main @see __VectorPair3__ */
    typedef VectorPair<3> VectorPair3;

    template <int N> class Transform;
    /** @main */
    template <> class Transform<3>;
    template <> class Transform<2>;
    /** @see __Transform2__ */
    typedef Transform<2> Transform2;
    /** @see __Transform3__ */
    typedef Transform<3> Transform3;

    template <int N> struct Matrix;
    typedef Matrix<3> Matrix3x3;

    typedef std::pair<double, double> range_t;

    /** Convert from radians to degrees */
    static inline double rad2degree(double x)
    {
      return (x * 180) / M_PI;
    }

    /** Convert from degrees to radians */
    static inline double degree2rad(double x)
    {
      return (x * M_PI) / 180;
    }

    /** Compute square */
    static inline double square(double x)
    {
      return x * x;
    }

    static inline double lp_floor(double x, double n)
    {
      return floor(x * n) / n;
    }

    static inline double lp_mod(double x, double n)
    {
      return x - trunc(x / n) * n;
    }
  }

  /** @module {Core}
      @short Optical system and components */
  namespace sys {
    using namespace goptical::sys;

    class Container;
    class system;
    class Element;
    class Surface;
    class Image;
    class Lens;
    class Stop;
    class Mirror;
    class Group;
    class OpticalSurface;
    class Source;
    class SourcePoint;
    class SourcePointInfinity;
    class Surface;
  }

  /** @module {Core}
      @short Ray tracer and light propagation */
  namespace trace {
    using namespace goptical::trace;

    class Distribution;
    class tracer;
    class Params;
    class Ray;
    class Result;
    class Element;
    class Sequence;

    typedef std::deque<Ray *> rays_queue_t;

  }

  /** @module {Core}
      @short 2d shapes used for surface contours */
  namespace shape {

    class Base;
    class Ring;
    class Infinite;

  }

  /** @module {Core}
      @short Physical properties of light */
  namespace light {

    class SpectralLine;
    class Ray;

  }

  /** @module {Core}
      @short material models for optical components */
  namespace material {
    using namespace goptical::material;

    template <enum AirFormula M> class Air;
    /** @see Air
        @short Air optical material model */
    typedef Air<AirBirch94Formula> AirBirch94;
    /** @see Air
        @short Air optical material model */
    typedef Air<AirKohlrausch68Formula> AirKohlrausch68;

    template <enum SellmeierModFormula m> class SellmeierMod;

    /** @see SellmeierMod
        @short Variation of the Sellmeier model for optical glass material */
    typedef SellmeierMod<SellmeierMod2Formula> SellmeierMod2;
    /** @see SellmeierMod
        @short Handbook of optics variation of the Sellmeier model for optical glass material */
    typedef SellmeierMod<Handbook1Formula> Handbook1;
    /** @see SellmeierMod
        @short Handbook of optics variation of the Sellmeier model for optical glass material */
    typedef SellmeierMod<Handbook2Formula> Handbook2;

    template <enum AbbeFormula m> class Abbe;
    /** @see Abbe
        @short Abbe model for optical glass material */
    typedef Abbe<AbbeVdFormula> AbbeVd;
    /** @see Abbe
        @short Abbe model for optical glass material */
    typedef Abbe<AbbeVeFormula> AbbeVe;

    class Base;
    class Catalog;
    class Vacuum;
    class Mirror;
    class Dielectric;
    class Metal;
    class Sellmeier;

  }

  /** @module {Core}
      @short data input/output and graphical rendering */
  namespace io {
    using namespace goptical::io;

    class Renderer;
    class Renderer2d;
    class RendererViewport;
    class RendererAxes;

    class RendererPlplot;
    class RendererSvg;
    class RendererGd;
    class RendererDxf;
    class RendererX11;
    class RendererOpengl;

    struct Rgb;
    struct Rgb;

  }

  /** @module {Core}
      @short Numerical data interpolation and plotting */
  namespace data {
    using namespace goptical::data;

    class Set;
    class Set1d;
    class DiscreteSetBase;
    class SampleSetBase;
    class Grid;
    class Plot;
    class Plotdata;
    
  }

  /** @module {Core}
      @short Surface curvature models */
  namespace curve {
    class Base;
    class Rotational;
    class Sphere;
    class Flat;
    class Conic;
    class Foucault;
    class Array;
  }

  /** @module {Core}
      @short Optical systems analysis tools */
  namespace analysis {
    class PointImage;
    class Spot;
    class Focus;
    class RayFan;
  }

}

#endif

