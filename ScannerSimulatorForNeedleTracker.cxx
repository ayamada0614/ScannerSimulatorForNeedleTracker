/*=========================================================================

  Program:   Open IGT Link -- ScannerSimulatorForNeedleTracker
  Module:    $RCSfile: $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved. 

  
  This software is based on Example for Imager Client Program and 
  modified by 
 
  Atsushi Yamada PhD, 
  Surgical Navigation and Robotics (SNR) Lab,
  Brigham and Women's Hospital and Harvard Medical School.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <iostream>
#include <math.h>
#include <cstdlib>
#include <cstdio>

#include "igtlOSUtil.h"
#include "igtlImageMessage.h"
#include "igtlClientSocket.h"

int GetTestImage(igtl::ImageMessage::Pointer& msg, const char* dir, int i);
void GetRandomTestMatrix(igtl::Matrix4x4& matrix);

int main(int argc, char* argv[])
{
  //------------------------------------------------------------
  // Parse Arguments

  if (argc != 5) // check number of arguments
    {
    // If not correct, print usage
    std::cerr << "Usage: " << argv[0] << " <hostname> <port> <fps> <imgdir>"    << std::endl;
    std::cerr << "    <hostname> : IP or host name"                    << std::endl;
    std::cerr << "    <port>     : Port # (18944 in Slicer default)"   << std::endl;
    std::cerr << "    <fps>      : Frequency (fps) to send coordinate" << std::endl;
    std::cerr << "    <imgdir>   : file directory, where \"igtlTestImage[1-5].raw\" are placed." << std::endl;
    std::cerr << "                 (usually, in the Examples/Imager/img directory.)" << std::endl;
    exit(0);
    }

  char*  hostname = argv[1];
  int    port     = atoi(argv[2]);
  double fps      = atof(argv[3]);
  int    interval = (int) (1000.0 / fps);
  char*  filedir  = argv[4];

  //------------------------------------------------------------
  // Establish Connection
  igtl::ClientSocket::Pointer socket;
  socket = igtl::ClientSocket::New();
  int r = socket->ConnectToServer(hostname, port);

  if (r != 0)
    {
    std::cerr << "Cannot connect to the server." << std::endl;
    exit(0);
    }

  
  // ayamada 08102012
  int count = 0;
  
  //------------------------------------------------------------
  // loop
  for (int i = 0; i < 100; i ++)
    {

    //------------------------------------------------------------
    // size parameters
    int   size[]     = {102, 128, 1};       // image dimension
    float spacing[]  = {1.0, 1.0, 5.0};     // spacing (mm/pixel)
    int   svsize[]   = {102, 128, 1};       // sub-volume size
    int   svoffset[] = {0, 0, 0};           // sub-volume offset
    //int   scalarType = igtl::ImageMessage::TYPE_USHORT;// scalar type
    int   scalarType = igtl::ImageMessage::TYPE_UINT8;// scalar type

    //------------------------------------------------------------
    // Create a new IMAGE type message
    igtl::ImageMessage::Pointer imgMsg = igtl::ImageMessage::New();
    imgMsg->SetDimensions(size);
    imgMsg->SetSpacing(spacing);
    imgMsg->SetScalarType(scalarType);
    
    // ayamada 08102012
    if(count == 0)
    {
      imgMsg->SetDeviceName("ImagerClient-0");
      count++;
    }else{
      imgMsg->SetDeviceName("ImagerClient-1");
      count = 0;      
    }
      
    imgMsg->SetSubVolume(svsize, svoffset);
    imgMsg->AllocateScalars();

    //------------------------------------------------------------
    // Set image data (See GetTestImage() bellow for the details)
    GetTestImage(imgMsg, filedir, i % 10);

    //------------------------------------------------------------
    // Get random orientation matrix and set it.
    igtl::Matrix4x4 matrix;
    GetRandomTestMatrix(matrix);
    imgMsg->SetMatrix(matrix);

    //------------------------------------------------------------
    // Pack (serialize) and send
    imgMsg->Pack();
    socket->Send(imgMsg->GetPackPointer(), imgMsg->GetPackSize());

    igtl::Sleep(interval); // wait

    }

  //------------------------------------------------------------
  // Close connection
  socket->CloseSocket();

}


//------------------------------------------------------------
// Function to read test image data
int GetTestImage(igtl::ImageMessage::Pointer& msg, const char* dir, int i)
{

  //------------------------------------------------------------
  // Check if image index is in the range
  if (i < 0 || i >= 10) 
    {
    std::cerr << "Image index is invalid." << std::endl;
    return 0;
    }

  //------------------------------------------------------------
  // Generate path to the raw image file
  char filename[128];
  //sprintf(filename, "%s/igtlTestImage%d.raw", dir, i+1);
  sprintf(filename, "%s/IMc%d.raw", dir, i+1);
  std::cerr << "Reading " << filename << "...";

  //------------------------------------------------------------
  // Load raw data from the file
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
    {
    std::cerr << "File opeining error: " << filename << std::endl;
    return 0;
    }
  int fsize = msg->GetImageSize();
  size_t b = fread(msg->GetScalarPointer(), 1, fsize, fp);

  fclose(fp);

  std::cerr << "done." << std::endl;

  return 1;
}

//------------------------------------------------------------
// Function to generate random matrix.
void GetRandomTestMatrix(igtl::Matrix4x4& matrix)
{
  matrix[0][0] = 1.0;  matrix[1][0] = 0.0;  matrix[2][0] = 0.0; matrix[3][0] = 0.0;
  matrix[0][1] = 0.0;  matrix[1][1] = -1.0;  matrix[2][1] = 0.0; matrix[3][1] = 0.0;
  matrix[0][2] = 0.0;  matrix[1][2] = 0.0;  matrix[2][2] = 1.0; matrix[3][2] = 0.0;
  matrix[0][3] = 0.0;  matrix[1][3] = 0.0;  matrix[2][3] = 0.0; matrix[3][3] = 1.0;
  
  igtl::PrintMatrix(matrix);
}

