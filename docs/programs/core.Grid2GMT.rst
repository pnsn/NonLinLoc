| 

Grid2GMT - location results or 3D grid data to GMT command script
=================================================================

**Grid2GMT** writes ASCII, GMT command scripts which will generate
postscript plots of event location results and sections of 3D grid
files.

.. raw:: html

   <div class="color">

 

.. raw:: html

   </div>

`Overview <#_overview_>`__ - `Running the program-Input <#_running_>`__
- `Output <#_output_>`__ - `[NonLinLoc Home] <./index.html>`__

.. raw:: html

   <div class="color">

 

.. raw:: html

   </div>

Overview
--------

The Grid2GMT utility generates and runs an ASCII, GMT command script for
plotting sections and plan views of 3D grid files and location misfit,
PDF and "scatter-cloud" results. When run, the GMT command script
creates a postscript file which can be viewed on screen or printed.
Running the GMT command script requires the intallation of the `GMT
plotting package <http://gmt.soest.hawaii.edu/>`__.

For flexibility, the Grid2GMT utility takes most of its parameters from
the command line. However, some parameters are obtained from the
`Generic Statements section <./control.html#_generic_>`__ of the Input
Control File.

The default GMT script plots the views in the transformed X-Y
coordinates. Horizontal (plan) view only plots can also be plotted in
geographic lat-long coordinates. See the notice and instructions in the
\*.gmt script file generated by Grid2GMT for more details.

Grid2GMT can be somewhat cumbersome and confusing to use because it
combines several functions (i.e. plotting sections and location
results), it requires several paths and filenames, and it does not have
as sophisticated error checking as other NonLinLoc programs.

.. raw:: html

   <div class="color">

 

.. raw:: html

   </div>

Running the program - Input
---------------------------

The Grid2GMT utility takes a number of command line arguments and has
three basic modes - (1) Vertical cross section (``V``), (2) Horizontal
section (``H``), and (3) Location (``L``) which generates a horizontal
section and two vertical sections through the maximum likelihood point
of an event location.

If a GMT-cpt file named ``Grid2GMT.cpt`` is present in the directory in
which the GMT script is run, it will be used for the plot and scalebar
contours levels and colors for all plot types that use contouring.
**(ver 2.0)**

If a GMT script file named ``Grid2GMT.?.gmt``, where ``? = H, X, Y`` or
``V`` is present in the directory in which the main GMT script is run,
it will be run for the corresponding view (``H, X, Y`` or ``V``)after
all standard GMT script is run for that view. **(ver 2.0)**

Synopsis:

    Vertical cross section (``V``):

    | ``Grid2GMT InputControlFile GridRoot Outroot V PlotType iX1 iY1 iX2 iY2``

    or, Vertical cross section lat/long (``VL``): **(ver 2.0)**

    | ``Grid2GMT InputControlFile GridRoot Outroot VL PlotType lat1 long1 lat2 long2``

    or, Horizontal section (``H``):

    | ``Grid2GMT InputControlFile GridRoot Outroot H PlotType iZ``

    or, Location (``L``, horizontal section and 2 vertical sections):

    ``Grid2GMT InputControlFile GridRoot Outroot L PlotType``

**Parameters: (general)**

 ``InputControlFile`` (*chars*)
    specifies the complete path and filename for an `Input Control
    File <./control.html>`__ with certain required and optional
    statements specifying program parameters and file names. To run
    Grid2GMT the `Generic Statements
    section <./control.html#_generic_>`__ of the Input Control File must
    contain the ``CONTROL`` and ``TRANS`` (Geographic Transformation)
    statements; it may optionally contain one or more ``MAPLINE``
    (Geographic Mapline) statements; and may contain a ``MAPTRANS``
    (Geographic Transformation for Grid2GMT plotting), or ``MAPGRID``
    (Grid region for Grid2GMT plotting), statements **(ver 2.3)**. If
    maplines are specified, they will be transformed if necessary and
    plotted on horizontal views. If a ``MAPTRANS`` statement is
    specified, it will be used for plotting, regardless if a ``TRANS``
    statement is present in the control file. If a ``MAPGRID`` statement
    is specified, it will be used for plotting, regardless if any
    ``LOCGRID`` specification is given in any hypocenter files.
 ``GridRoot`` (*chars*)
    full or relative path and *root* name (no extension) for a `3D
    Grid <formats.html#_grid_>`__ (modes ``V`` or ``H``), or for an
    event location (mode ``L``). Mulitple root names may be specified
    using standard UNIX "wild-card" characters (``*`` and ``?``);
    however, if any "wild-card" characters are used then the path and
    root name must be enclosed in double quotes (") to prevent the shell
    from evaluating the "wild-card" characters.
 ``Outroot`` (*chars*)
    full or relative path and optional root name for output and
    temporary files.
 ``PlotType``
    type of data to plot, options are:

    ``G`` - Plot grided data as a contoured, color grid. For locations
    (``L``), also plots location statistics.

    ``S`` - Plot location "scatter-cloud" results. For locations
    (``L``), also plots location statistics.

    ``Ennndx`` - Plot location statistics (Maximum Likelihood location
    and/or traditional, Gaussian/Normal Expectation and Confidence
    Ellipsoid). ``nnndx`` is a code of: three *integers* with values
    ``n`` = ``0`` (no) or ``1`` (yes) specifying which statistics to
    plot (Maximum Likelhood point, Expectation point and Confidence
    Ellipsoid, respectively), and an optional *float* ``dx`` which
    specifies a maximum distance in km between the Expectation and
    Maximum Likelihood points allowed for each event to be plotted.

    ``M`` **(ver 2.3)** - Plot focal mechanisms.

    ``Rphases/scale`` **(ver 2.3)** - Plot phase residuals as symbols
    with a size proportional to residual magnitude and with type and
    color dependent on phase. ``phases`` is a set of phase codes, i.e.
    ``P`` or ``SnSg`` specifying which phases to plot. ``scale``
    (*float*) specifyies the relative size of the residual symbols.

**Parameters: (Vertical cross section - ``V``)**

 ``iX1 iY1 iX2 iY2`` (*integers*)
    the minimum and maximum grid indices (``0`` to ``Num-1``, where
    ``Num`` is the number of grid nodes) for the *x* and *y* directions.
    This option plots sections parallel to the cooridinate axes and
    oblique sections **(ver 2.0)**. Set ``iX1``\ =\ ``iX2`` and
    ``iY1``\ <``iY2`` for a section in the *y,z* plane, or
    ``iY1``\ =\ ``iY2`` and ``iX1``\ <``iX2`` for a section in the *x,z*
    plane. The full grid range in the *z* direction is always plotted.

**Parameters: (Vertical cross section lat/long - ``VL``)** **(ver 2.0)**

 ``lat1 long1 lat2 long2`` (*floats*)
    the latitude and longitude of the left (1) and right (2) end of the
    cross section (all lat/long locations must be within the 3D grid) .
    This option plots sections parallel to the cooridinate axes and
    oblique sections. The lat/long locations are coverted to grid
    locations using the transformation specified in the ``TRANS``
    statement of the ``InputControlFile``. The full grid range in the
    *z* direction is always plotted.

**Parameters: (Horizontal cross section - ``H``)**

 ``iZ`` (*integer*)
    the grid index (``0`` to ``zNum-1``, where ``zNum`` is the number of
    grid nodes) for the section level in the *z* direction. Currently
    this option only plots sections parallel to the *x,y* cooridinate
    axes. The full grid range in the *x* and *y* directions is always
    plotted.

**Parameters: (Location - ``L``)**

 (*no additional parameters*)
    The full grid range in the *x*, *y* and *z* directions is always
    plotted.

**Notes:**

#. See the `Definitions section <./control.html#_definitions_>`__ of the
   NonLinLoc Control File documentation for more information on
   datatypes.
#. Grid2GMT uses cm units, thus for proper execution of the GMT command
   script, the MEASURE\_UNIT entry in the .gmtdefaults file must be set
   to cm (i.e. ``MEASURE_UNIT = cm``)
#. The *x, y* and *z* coordinate limits and the number of grid nodes is
   determined by the grid origin and dimensions in the grid header file
   corresponding to the ``GridRoot`` root name. For plotting event
   locations which have no corresponding grid files, create a `3D
   Grid <formats.html#_grid_>`__ header file named (``GridRoot.hdr``)
   with the desired grid origin and dimensions.
#. The plotted ellipsoids are 2D projections of the Confidence
   Ellipsoids as calculated by the NLLoc location program (see
   `Inversion section <./NLLoc.html#_inversion_>`__ of the NLLoc program
   documentation); the 2D projections DO NOT represent the 2D, marginal
   distributions of the 3D Confidence Ellipsoids.

**Examples:**

#. ``Grid2GMT run/nlloc_sample.in  time/layer.P.AURF.time gmt/ V G 0 0 0 301``

   Using Input Control File ``run/nlloc_sample.in``, creates a GMT
   script for the 3D grid travel-time file with root name
   ``time/layer.P.AURF.time``. The GMT script and temporary files are
   written to directory ``./gmt/``. The script will plot a Vertical
   cross section (``V``), showing the travel-time grid (``G``) for grid
   nodes = ``0`` in the *x* direction and between grid nodes ``0`` and
   ``301`` in the *y* direction. (`View resulting
   plot <./Grid2GMT_ex1.gif>`__)

#. ``Grid2GMT run/nlloc_sample.in "loc/vinti.*.*.grid0.loc" gmt/ L S``

   Using Input Control File ``run/nlloc_sample.in``, creates a GMT
   script for each location with root name ``"loc/dur.*.*.grid0.loc"``.
   The GMT script and temporary files are written to directory
   ``./gmt/``. For each location, the script will plot a Location
   (``L``, horizontal section and 2 vertical sections), showing the
   location PDF "scatter-cloud" samples (``S``). (`View one of the
   resulting plots <./Grid2GMT_ex2.gif>`__)

#. ``Grid2GMT run/nlloc_sample.in loc/vinti.sum.grid0.loc gmt/ L E111``

   Using Input Control File ``run/nlloc_sample.in``, creates a GMT
   script for the summary hypocenter file with root name
   ``loc/vinti.sum.grid0.loc``. The GMT script and temporary files are
   written to directory ``./gmt/``. The script will plot a Location
   (``L``, horizontal section and 2 vertical sections), showing all
   location statistics (``E111``, Maximum Likelihood location and
   traditional, Gaussian/Normal Expectation and projection of Confidence
   Ellipsoid), with no restriction on the maximum distance between the
   Expectation and Maximum Likelihood points (``dx`` omitted). Note that
   for the summary hypocenter files there are no corresponding grid
   files, thus a `3D Grid <formats.html#_grid_>`__ header file named
   ``dur.sum.grid0.loc.hdr`` with the desired grid origin and dimensions
   had to be created. ( `View resulting plot <./Grid2GMT_ex3.gif>`__)

#. ``Grid2GMT run/nlloc_sample.in loc/vinti gmt/ L S``
   Using Input Control File ``run/nlloc_sample.in``, creates a GMT
   script for the summary hypocenter/scatter file with root name
   ``loc/vinti`` created by program `LocSum <./LocSum.html>`__. The GMT
   script and temporary files are written to directory ``./gmt/``. The
   script will plot a Location (``L``, horizontal section and 2 vertical
   sections), showing the "scatter-cloud" samples (``S`` for all events.
   (`View resulting plot <./Grid2GMT_ex4.gif>`__)
#. ``Grid2GMT run/dur.in dur.19950725.111732.grid2.loc gmt/ L G``

   Using Input Control File ``run/dur.in``, creates a GMT script for the
   3D grid grid-search PDF file in the current directory with root name
   ``dur.19950725.111732.grid2.loc``. The GMT script and temporary files
   are written to directory ``./gmt/``. The script will plot a Location
   (``L``, horizontal section and 2 vertical sections), showing the
   location PDF as contoured, color confidence levels (``S``), along
   with location statistics (Maximum Likelihood location and
   traditional, Gaussian/Normal Expectation and projection of Confidence
   Ellipsoid). (`View resulting plot <./Grid2GMT_ex5.gif>`__)

.. raw:: html

   <div class="color">

 

.. raw:: html

   </div>

Output
------

The Grid2GMT utility generates and runs an ASCII, GMT command script
with the name ``Outroot/GridRoot.gmt``. When run, this GMT command
script creates a postscript file which can be viewed on screen with a
postscript viewer, or printed on a postscript printer. Running the GMT
command script requires the intallation of the `GMT plotting
package <http://gmt.soest.hawaii.edu/>`__.

The GMT command script can be edited and elements of the script
extracted to produce custom GMT plots.

.. raw:: html

   <div class="color">

 

.. raw:: html

   </div>

Back to `the NonLinLoc site Home page <./index.html>`__.

*Anthony Lomax*
