/*
 * Copyright (C) 2000 Anthony Lomax <lomax@geoazur.unice.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */


/*   asc2grid.c

	Program to convert ASCII grid to Grid3D format grid

*/

//------------------------------------------------------------/
// Anthony Lomax           | email: lomax@geoazur.unice.fr     /
// UMR Geosciences Azur    | web: www-geoazur.unice.fr/~lomax /
// 250 Rue Albert Einstein | tel: 33 (0) 4 93 95 43 25        /
// 06560 Valbonne, FRANCE  | fax: 33 (0) 4 93 65 27 17        /
//------------------------------------------------------------/


/*
	history:

	ver 01    04Nov1997  AJL  Original version


.........1.........2.........3.........4.........5.........6.........7.........8

*/



#include "../src/GridLib.h"


// defines

#define MAX_NUM_INPUT_FILES 500


// globals


// functions

int doAscii2Grid(char* asciiFileName, char* gridFileRootName,
                int decimFact, float velZeroReplace);



/*** Program to convert ASCII grid to Grid3D format grid */

#define PNAME  "GridProc"


main(int argc, char *argv[])
{

	int istat, narg;
	int decimFact;
	char asciiFileName[FILENAME_MAX];
	char gridFileRootName[FILENAME_MAX];
	float velZeroReplace;


	// set program name

	strcpy(prog_name, PNAME);


	// check command line for correct usage

	fprintf(stdout, "\n%s Arguments: ", prog_name);
	for (narg = 0; narg < argc; narg++)
		fprintf(stdout, "<%s> ", argv[narg]);
	fprintf(stdout, "\n");

	if (argc < 5)   {
		nll_puterr("ERROR wrong number of command line arguments.");
		disp_usage(PNAME, "<asciiGridfile> <outputGridfileRoot> <decim> <velZeroReplace>");
	exit(-1);
        }

        sscanf(argv[1], "%s", asciiFileName);
        sscanf(argv[2], "%s", gridFileRootName);
        sscanf(argv[3], "%d", &decimFact);
        sscanf(argv[4], "%f", &velZeroReplace);

        if ((istat = doAscii2Grid(asciiFileName, gridFileRootName,
		decimFact, velZeroReplace)) < 0) {
                nll_puterr("ERROR doing ADD process.");
 		exit(-1);
	}

	exit(0);

}



int doAscii2Grid(char* asciiFileName, char* gridFileRootName,int decimFact,  float velZeroReplace)
{
	int istat, narg;
	int nx, ny, nz, nskip;
	int nx1, ny1, nz1;
	int numx, numy, numz;
	int ndxx, ndxy, ndxz;
	float slowness, velMin;
 float x, y, z;

 	char velGridFleRootName[FILENAME_MAX];
	FILE *fp_grid_ascii;

	GridDesc grid_out, grid_vel_out;


	// open ascii grid file

	if ((fp_grid_ascii = fopen(asciiFileName, "r")) == NULL)
	{
		nll_puterr("ERROR opening ascii grid file.");
		return(-1);
	}


	// cread and initialize output grid

	grid_out.buffer = NULL;
	grid_out.array;			/* array access to buffer */
	grid_out.autox = grid_out.autoy = grid_out.autoz = 0;
        grid_out.type = GRID_SLOW_LEN;
	strcpy(grid_out.chr_type, "SLOW_LEN");
	strcpy(grid_out.title, asciiFileName);
	grid_out.sum = 0.0;
	grid_out.iSwapBytes = 0;

	istat = fscanf(fp_grid_ascii, "%lf %*lf %lf %*lf %lf %*lf %lf %lf %lf %d %d %d",
		&grid_out.origx, &grid_out.origy, &grid_out.origz,
		&grid_out.dx, &grid_out.dy, &grid_out.dz,
	        &numx, &numy, &numz
        );
	if (istat != 9) {
		nll_puterr("ERROR: reading grid dimensions.\n");
		return(-1);
	}
	grid_out.dx *= decimFact;
	grid_out.dy *= decimFact;
	grid_out.dz *= decimFact;
	numx++;
	numy++;
//numz = 40;
	numz++;
	grid_out.numx = numx / decimFact + 1;
	grid_out.numy = numy / decimFact + 1;
	grid_out.numz = numz / decimFact + 1;

	if (grid_out.dx != grid_out.dy || grid_out.dy != grid_out.dz
	                || grid_out.dz != grid_out.dx) {
		nll_puterr("ERROR: x, y and z cell sizes not equal.\n");
		return(-1);
	}

        display_grid_param(&grid_out);

	// allocate slowness grid
	printf("Allocating memory for output slowness grid buffer...\n");
	grid_out.buffer = AllocateGrid(&grid_out);
	if (grid_out.buffer == NULL) {
		nll_puterr("ERROR: allocating memory for output slowness grid buffer.\n");
		return(-1);
	}

	// create slowness grid array access pointers
	printf("Creating array for accessing output slowness grid buffer...\n");
	grid_out.array = CreateGridArray(&grid_out);
	if (grid_out.array == NULL) {
		nll_puterr("ERROR: creating array for accessing output slowness grid buffer.\n");
		return(-1);
	}

	grid_vel_out = grid_out;
        grid_vel_out.type = GRID_VELOCITY;
	strcpy(grid_vel_out.chr_type, "VELOCITY");

	// allocate velocity grid
	printf("Allocating memory for output velocity grid buffer...\n");
	grid_vel_out.buffer = AllocateGrid(&grid_vel_out);
	if (grid_vel_out.buffer == NULL) {
		nll_puterr("ERROR: allocating memory for output velocity grid buffer.\n");
		return(-1);
	}

	// create velocity grid array access pointers
	printf("Creating array for accessing output velocity grid buffer...\n");
	grid_vel_out.array = CreateGridArray(&grid_vel_out);
	if (grid_vel_out.array == NULL) {
		nll_puterr("ERROR: creating array for accessing output velocity grid buffer.\n");
		return(-1);
	}

	// convert input ascii grid into output grid
	printf("Converting input ascii grid to Grid3D grid...\n");
	velMin = 1.0e30;
	ndxz = 0;
	for (nz = 0; nz < numz; nz += decimFact) {
		//fprintf(stderr, "\rnz=%d/%d", nz, numz);
		ndxy = 0;
		for (ny = 0; ny < numy; ny += decimFact) {
		        fprintf(stderr, "\rz=%d/%d y=%d/%d",
					ndxz, numz / decimFact - 1, ndxy, numy / decimFact - 1);
			ndxx = 0;
			for (nx = 0; nx < numx; nx += decimFact) {
			        fscanf(fp_grid_ascii, "%f %f %f %f", &x, &y, &z, &slowness);
				if (slowness < 1.0e-6)
				        slowness = 1.0 / velZeroReplace;
				else if (slowness > 1.0 / velMin)
				        velMin = 1.0 / slowness;
                                grid_out.array[ndxx][ndxy][ndxz] = (float) (grid_out.dx * slowness);
                                grid_vel_out.array[ndxx][ndxy][ndxz] = (float) (1.0 / slowness);
			        ndxx++;
				nskip = decimFact - 1;
				nx1 = nx;
			        while (--nskip >= 0 && ++nx1 < numx)
			                fscanf(fp_grid_ascii, "%*f %*f %*f %*f");
			}
			ndxx--;
			if (ndxx >= grid_out.numx) {
				printf("ERROR: ndxx (%d) >= grid_out.numx.(%d)\n", ndxx, grid_out.numx);
				return(-1);
			}
			ndxy++;
			nskip = decimFact - 1 ;
			ny1 = ny;
			while (--nskip >= 0 && ++ny1 < numy)
				for (nx = 0; nx < numx; nx++)
					fscanf(fp_grid_ascii, "%*f %*f %*f %*f");
		}
		ndxy--;
		if (ndxy >= grid_out.numy) {
			printf("ERROR: ndxy (%d) >= grid_out.numy.(%d)\n", ndxy, grid_out.numy);
			return(-1);
		}
		printf("Last read: (%f,%f,%f) slowness=%f vel=%f\n", x, y, z, slowness, 1.0 / slowness);
		printf("Last index: (%d,%d,%d)\n", ndxx, ndxy, ndxz);
		ndxz++;
		nskip = decimFact - 1;
		nz1 = nz;
 fprintf(stderr, "\nIN nz1=%d/%d nskip %d numz %d\n", nz1, nz, nskip, numz);
		while (--nskip >= 0 && ++nz1 < numz)
			for (ny = 0; ny < numy; ny++)
				for (nx = 0; nx < numx; nx++)
					fscanf(fp_grid_ascii, "%*f %*f %*f %*f");
 fprintf(stderr, "OUT nz1=%d/%d nskip %d numz %d\n", nz1, nz, nskip, numz);
        }
	ndxz--;
	if (ndxz >= grid_out.numz) {
		printf("ERROR: ndxz (%d) >= grid_out.numz.(%d)\n", ndxz, grid_out.numz);
	}
	printf("\ndone!  velMin=%f velZeroReplace=%f\n", velMin, velZeroReplace);
	printf("Last read: (%f,%f,%f) slowness=%f vel=%f\n", x, y, z, slowness, 1.0 / slowness);
	printf("Last index: (%d,%d,%d)\n", ndxx, ndxy, ndxz);


	// save grids to disk

	printf("Writing output slowness grid to disk...\n");
	if ((istat = WriteGrid3dBuf(&grid_out, NULL, gridFileRootName, "mod")) < 0) {
		nll_puterr("ERROR: writing output grid to disk.\n");
		return(-1);
	}
	printf("Writing output velocity grid to disk...\n");
	sprintf(velGridFleRootName, "%s_vel", gridFileRootName);
	if ((istat = WriteGrid3dBuf(&grid_vel_out, NULL, velGridFleRootName, "mod")) < 0) {
		nll_puterr("ERROR: writing output grid to disk.\n");
		return(-1);
	}


	fclose(fp_grid_ascii);

	return(0);

}



//------------------------------------------------------------/
// Anthony Lomax           | email: lomax@geoazur.unice.fr     /
// UMR Geosciences Azur    | web: www-geoazur.unice.fr/~lomax /
// 250 Rue Albert Einstein | tel: 33 (0) 4 93 95 43 25        /
// 06560 Valbonne, FRANCE  | fax: 33 (0) 4 93 65 27 17        /
//------------------------------------------------------------/

