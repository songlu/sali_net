#include "stdafx.h"
#include "hdf5.h"
#define FILE_NAME1 "file.h5"

#include <iostream>
#include <string>
#include "H5Cpp.h"
#ifndef H5_NO_NAMESPACE
    using namespace H5;
#endif
const H5std_string	FILE_NAME( "dset.h5" );
const H5std_string	DATASET_NAME( "dset" );
const int DIM0 = 4;
const int DIM1 = 6;
void hdf5_test1() {
	hid_t       file_id;   /* file identifier */
    herr_t      status;

    /* Create a new file using default properties. */
    file_id = H5Fcreate(FILE_NAME1, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	
    /* Terminate access to the file. */
    status = H5Fclose(file_id); 
}

int hdf5_test2() {
	// Try block to detect exceptions raised by any of the calls inside it 
      // Turn off the auto-printing when failure occurs so that we can
      // handle the errors appropriately
 
      // Create a new file using the default property lists. 
     
      H5File file( FILE_NAME, H5F_ACC_TRUNC );

      // Create the data space for the dataset.
       
      hsize_t dims[2];              // dataset dimensions
      dims[0] = 4;
      dims[1] = 6;
      DataSpace dataspace ( 2, dims );

      IntType datatype( PredType::STD_I32BE );

      // Create the dataset.     
  
      DataSet dataset = file.createDataSet( DATASET_NAME, datatype, dataspace );

   return 0;  // successfully terminated
}

int hdf5_test3()
{
	// Data initialization.
    
   int i, j;
   int data[DIM0][DIM1];          // buffer for data to write

   for (j = 0; j < DIM0; j++)
      for (i = 0; i < DIM1; i++)
	 data[j][i] = i * 6 + j + 1;

   // Try block to detect exceptions raised by any of the calls inside it

      // Turn off the auto-printing when failure occurs so that we can
      // handle the errors appropriately
       
      Exception::dontPrint();

      // Open an existing file and dataset.

      H5File file( FILE_NAME, H5F_ACC_RDWR );
      DataSet dataset = file.openDataSet( DATASET_NAME );

      // Write the data to the dataset using default memory space, file
      // space, and transfer properties.

      dataset.write( data, PredType::NATIVE_INT );

   return 0;  // successfully terminated
}

const H5std_string	FILE_NAME4( "subset.h5" );
const H5std_string	DATASET_NAME4( "IntArray" );

#define RANK            2

#define DIM0_SUB        3            // subset dimensions
#define DIM1_SUB        4 

#define DIM0            8            // size of dataset
#define DIM1           10
using namespace std;
int hdf5_test4(){
	int     i,j;
   int     data[DIM0][DIM1], sdata[DIM0_SUB][DIM1_SUB], rdata[DIM0][DIM1];

   // Try block to detect exceptions raised by any of the calls inside it
      
      // Turn off the auto-printing when failure occurs so that we can
      // handle the errors appropriately

      // ---------------------------------------------------
      // Create a new file using the default property lists. 
      // Then create a dataset and write data to it. 
      // Close the file and dataset.
      // ---------------------------------------------------
     
      H5File *file = new H5File( FILE_NAME4, H5F_ACC_TRUNC );

      hsize_t dims[2];             
      dims[0] = DIM0;
      dims[1] = DIM1;
      DataSpace *dataspace = new DataSpace ( RANK, dims );

      DataSet *dataset = new DataSet( file->createDataSet( DATASET_NAME4, 
                                      PredType::STD_I32BE, *dataspace ) );


      for (j = 0; j < DIM0; j++) {
          for (i = 0; i < DIM1; i++)
              if (i< (DIM1/2))
                 data[j][i] = 1;
              else
                 data[j][i] = 2;
           }

      dataset->write( data, PredType::NATIVE_INT );

      cout << endl << "Data Written to File:" << endl;
      for (j = 0; j < DIM0; j++) {
         for (i = 0; i < DIM1; i++)
            cout << " " <<  data[j][i];
         cout << endl;
      }

      delete dataspace;
      delete dataset;
      delete file;

      // ---------------------------------------------------
      // Reopen the file and dataset and write a subset of
      // values to the dataset.
      // ---------------------------------------------------

      hsize_t offset[2], count[2], stride[2], block[2];
      hsize_t dimsm[2];

      file = new H5File( FILE_NAME4, H5F_ACC_RDWR );
      dataset = new DataSet( file->openDataSet( DATASET_NAME4) );

      // Specify size and shape of subset to write. 

      offset[0] = 1;
      offset[1] = 2;

      count[0]  = DIM0_SUB;
      count[1]  = DIM1_SUB;

      stride[0] = 1;
      stride[1] = 1;

      block[0] = 1;
      block[1] = 1;
  
      // Define Memory Dataspace. Get file dataspace and select
      // a subset from the file dataspace.

      dimsm[0] = DIM0_SUB;
      dimsm[1] = DIM1_SUB;

      DataSpace *memspace = new DataSpace (RANK, dimsm, NULL);

      dataspace = new DataSpace( dataset->getSpace () );
      dataspace->selectHyperslab( H5S_SELECT_SET, count, offset, stride, block ); 

      // Write a subset of data to the dataset, then read the
      // entire dataset back from the file.

      cout << endl << "Write subset to file specifying: " << endl;
      cout << "  offset=1x2 stride=1x1 count=3x4 block=1x1" << endl;
      for (j = 0; j < DIM0_SUB; j++) {
         for (i = 0; i < DIM1_SUB; i++)
            sdata[j][i] = 5;
      } 
      
      dataset->write( sdata, PredType::NATIVE_INT, *memspace, *dataspace );
      dataset->read( rdata, PredType::NATIVE_INT );

 
      cout << endl << "Data in File after Subset is Written:" << endl;
      for (i = 0; i < DIM0; i++) {
         for (j = 0; j < DIM1; j++) 
            cout << " " <<  rdata[i][j];
         cout << endl;
      }
      cout << endl;

      delete dataspace;
      delete memspace;
      delete dataset;
      delete file;

   return 0;  // successfully terminated
}

#define FILE_NAME5        "subset.h5"
#define DATASET_NAME5 "IntArray" 
void hdf5_test5()
{
    hsize_t     dims[2], dimsm[2];   
    int         data[DIM0][DIM1];           /* data to write */
    int         sdata[DIM0_SUB][DIM1_SUB];  /* subset to write */
    int         rdata[DIM0][DIM1];          /* buffer for read */
 
    hid_t       file_id, dataset_id;        /* handles */
    hid_t       dataspace_id, memspace_id; 

    herr_t      status;                             
   
    hsize_t     count[2];              /* size of subset in the file */
    hsize_t     offset[2];             /* subset offset in the file */
    hsize_t     stride[2];
    hsize_t     block[2];
    int         i, j;

    
    /*****************************************************************
     * Create a new file with default creation and access properties.*
     * Then create a dataset and write data to it and close the file *
     * and dataset.                                                  *
     *****************************************************************/

    file_id = H5Fcreate (FILE_NAME5, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    dims[0] = DIM0;
    dims[1] = DIM1;

	hsize_t dimsnew[3];
	dimsnew[0] = DIM0;
	dimsnew[1] = DIM1/2;
	dimsnew[2] = 2;
    dataspace_id = H5Screate_simple (3, dimsnew, NULL); 
	//dataspace_id = H5Screate(H5S_SIMPLE);

    dataset_id = H5Dcreate2 (file_id, DATASET_NAME5, H5T_STD_I32BE, dataspace_id,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

	/*
    for (j = 0; j < DIM0; j++) {
	for (i = 0; i < DIM1; i++)
            if (i< (DIM1/2))
	       data[j][i] = 1;
            else
               data[j][i] = 2;
    }     
	
    status = H5Dwrite (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
                      H5P_DEFAULT, data);
    */
	int * data2;
	data2 = new int[DIM0*DIM1];
	for (j = 0; j < DIM0; j++) {
	for (i = 0; i < DIM1; i++)
            if (i< (DIM1/2))
	            data2[j*DIM1+i] = 1;
            else
                data2[j*DIM1+i] = 2;
    }     
	
    status = H5Dwrite (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
                      H5P_DEFAULT, data2);

    printf ("\nData Written to File:\n");
    for (i = 0; i<DIM0; i++){
       for (j = 0; j<DIM1; j++)
           printf (" %i", data2[i*DIM1+j]);
       printf ("\n");
    }
    status = H5Sclose (dataspace_id);
    status = H5Dclose (dataset_id);
    status = H5Fclose (file_id);


    /*****************************************************
     * Reopen the file and dataset and write a subset of *
     * values to the dataset. 
     *****************************************************/

    file_id = H5Fopen (FILE_NAME5, H5F_ACC_RDWR, H5P_DEFAULT);
    dataset_id = H5Dopen2 (file_id, DATASET_NAME5, H5P_DEFAULT);

    /* Specify size and shape of subset to write. */

    offset[0] = 1;
    offset[1] = 2;

    count[0]  = DIM0_SUB;  
    count[1]  = DIM1_SUB;

    stride[0] = 1;
    stride[1] = 1;

    block[0] = 1;
    block[1] = 1;

    /* Create memory space with size of subset. Get file dataspace 
       and select subset from file dataspace. */

    dimsm[0] = DIM0_SUB;
    dimsm[1] = DIM1_SUB;
    memspace_id = H5Screate_simple (RANK, dimsm, NULL); 

    dataspace_id = H5Dget_space (dataset_id);
    status = H5Sselect_hyperslab (dataspace_id, H5S_SELECT_SET, offset,
                                  stride, count, block);

    /* Write a subset of data to the dataset, then read the 
       entire dataset back from the file.  */

    printf ("\nWrite subset to file specifying:\n");
    printf ("    offset=1x2 stride=1x1 count=3x4 block=1x1\n");
    for (j = 0; j < DIM0_SUB; j++) {
	for (i = 0; i < DIM1_SUB; i++)
	   sdata[j][i] = 5;
    }     

    status = H5Dwrite (dataset_id, H5T_NATIVE_INT, memspace_id,
                       dataspace_id, H5P_DEFAULT, sdata);
    
    status = H5Dread (dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
                       H5P_DEFAULT, rdata);

    printf ("\nData in File after Subset is Written:\n");
    for (i = 0; i<DIM0; i++){
       for (j = 0; j<DIM1; j++)
           printf (" %i", rdata[i][j]);
       printf ("\n");
    }

    status = H5Sclose (memspace_id);
    status = H5Sclose (dataspace_id);
    status = H5Dclose (dataset_id);
    status = H5Fclose (file_id);
 
}