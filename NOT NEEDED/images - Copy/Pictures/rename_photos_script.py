#!/usr/bin/env python3
"""
Photo Renaming Script for OuchiTaiko Project
Compares old and new markdown files to rename image files
"""

import os
import re
from pathlib import Path

# Mapping of old filenames to new filenames
RENAME_MAP = {
    "10000001000009F6000000CE4BA065654.png": "mainlogo.png",
    "1000000000000753000009C4E8EB3F27.jpg": "finishedside.jpg",
    "10000000000009C4000009A1F933F88D.jpg": "finishedface.jpg",
    "1000000000000753000009C464647684.jpg": "finishedfront.jpg",
    "1017A8F200011F380000DE6AE0BF0922.png": "circuitdiagram.png",
    "10000001000009F600000D02F3BF30D0.png": "pinouts.png",
    "100000000000080000000600006BD9A8.jpg": "blankboard.jpg",
    "10000001000003FF0000038627712BCD.png": "pinground.png",
    "100000010000023F000000D8085F9677.png": "trswiring.png",
    "100000000000060D000004FD52D8B4F8.jpg": "capacitors.jpg",
    "100000000000051B00000364BB6B5E07.jpg": "diodes.jpg",
    "10000001000001FC0000029AE2FB34AC.png": "diodesandcapacitors.png",
    "100000010000013F000001DF59A6E42F.png": "trsharness.png",
    "10000000000009AE00000C346364AD26.jpg": "mainboard.jpg",
    "100000010000046D00000220ECE6EDA1.png": "SDA.png",
    "100000010000046D00000220D210F258.png": "SCL.png",
    "100000010000046D0000022040573B96.png": "VCC.png",
    "100000010000046D000002207116B219.png": "GND.png",
    "1000000100000566000003318F39E808.png": "gamepad.png",
    "100000000000080000000600206460C4.jpg": "samplebuttons.jpg",
    "1000000000000800000006007E8007EB.jpg": "sampleground.jpg",
    "10000000000006FC00000794DEC0E2F7.jpg": "allbuttonsrear.jpg",
    "100000000000060000000800A4CE90BA.jpg": "baseplate.jpg",
    "100000000000080000000600B828828F.jpg": "kaholes.jpg",
    "100000000000050A000005E069D763B9.jpg": "kafront.jpg",
    "1000000000000600000008002B78DEAA.jpg": "kahole.jpg",
    "10000001000003A80000036583CFC771.png": "kagrommetout.png",
    "10000001000002B4000002693C216CE2.png": "kagrommetin.png",
    "10000001000002CE000002D668B30CE1.png": "kagrommettool.png",
    "100000010000024600000189C50C572B.png": "threadedinserts.png",
    "10000001000004DD000002D79118BD45.png": "nyloncut.png",
    "10000001000003B1000002DC0BD8FB16.png": "isolatorgrommetside.png",
    "10000001000003EC000002E263D51B16.png": "grommetmeasure.png",
    "1000000100000358000002DC0D4ED171.png": "grommetinstalled.png",
    "10000000000005E8000007E0FE99EF21.jpg": "sensorandhousing.jpg",
    "1000000000000A84000006586122FF5A.jpg": "trssolder.jpg",
    "10000000000005E8000007E09B34A737.jpg": "sensorsolder.jpg",
    "10000000000005E8000007E09D348BCE.jpg": "sensorinhousing.jpg",
    "10000000000005E8000007E05241CC8B.jpg": "sensorholding.jpg",
    "1000000000000A42000006B7AF84CCDB.jpg": "housinginsert.jpg",
    "100000000000060000000800F0D97183.jpg": "speakerbracket.jpg",
    "100000000000024B00000304BDC895E9.jpg": "bracketholes.jpg",
    "100000000000060000000800B528FD8E.jpg": "bracketbolts.jpg",
    "1000000000000600000008000289516B.jpg": "bracketconnected.jpg",
    "10000001000002210000026E92CF05A8.png": "wirerouteold.png",
    "couplers.JPG": "couplersold.JPG",
    "1000000100000223000002E922433EA6.png": "couplerstopold.png",
    "1000000100000224000002DF48CE7655.png": "trsbox.png",
    "1000000100000221000002E17277E344.png": "bothports.png",
    "1000000000000600000008008135383A.jpg": "insideboxall.jpg",
    "100000000000082900000EA3CFEA28D2.jpg": "frontbox.jpg",
    "1000000000000CF300000B20F5026C3E.jpg": "boxopenall.jpg",
    "1000000000000753000009C4DA4EE4D3.jpg": "drumcover.jpg",
    "100000010000053B000003083BE429C9.png": "downloadimage.png",
}

def rename_images():
    """
    Rename image files in the current directory based on the mapping above
    """
    # Use current directory where script is located
    folder_path = Path(__file__).parent
    
    print(f"🔍 Searching for images in: {folder_path.absolute()}\n")
    
    # Track statistics
    renamed_count = 0
    not_found_count = 0
    skipped_count = 0
    
    # Process each file in the rename map
    for old_name, new_name in RENAME_MAP.items():
        old_path = folder_path / old_name
        new_path = folder_path / new_name
        
        # Check if old file exists
        if not old_path.exists():
            print(f"⚠️  Not found: {old_name}")
            not_found_count += 1
            continue
        
        # Check if new file already exists
        if new_path.exists():
            print(f"⏭️  Skipped: {old_name} → {new_name} (destination already exists)")
            skipped_count += 1
            continue
        
        # Rename the file
        try:
            old_path.rename(new_path)
            print(f"✅ Renamed: {old_name} → {new_name}")
            renamed_count += 1
        except Exception as e:
            print(f"❌ Error renaming {old_name}: {e}")
    
    # Print summary
    print("\n" + "="*60)
    print("📊 SUMMARY:")
    print(f"   ✅ Successfully renamed: {renamed_count} files")
    print(f"   ⚠️  Not found: {not_found_count} files")
    print(f"   ⏭️  Skipped (already exists): {skipped_count} files")
    print(f"   📁 Total in map: {len(RENAME_MAP)} files")
    print("="*60)

if __name__ == "__main__":
    print("="*60)
    print("  OuchiTaiko Project - Photo Renaming Script")
    print("="*60)
    print()
    
    # Run the rename function
    rename_images()
    
    print("\n✨ Script complete!")
    input("\nPress Enter to exit...")
