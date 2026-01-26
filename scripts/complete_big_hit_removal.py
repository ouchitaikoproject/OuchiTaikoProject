#!/usr/bin/env python3
"""
Complete Big Hit Mode Removal Script
This script removes all Big Hit Mode references from the remaining files.
Run this from your OuchiTaikoProject directory.
"""

import os
import re
from pathlib import Path

def remove_big_hit_from_menu_cpp(content):
    """Remove all Big Hit references from Menu.cpp"""
    
    # Remove Gameplay menu Big Hit item
    content = re.sub(
        r'(\{"Gameplay\\nMods",\s*\{\{)"Big Hit\\nMode", Menu::Descriptor::Action::GotoPageDrumBigHitArcade\},\s*',
        r'\1',
        content
    )
    
    # Remove DrumBigHitArcade page definition (entire block)
    content = re.sub(
        r'    // Big Hit Mode.*?\n.*?\{Menu::Page::DrumBigHitArcade,.*?\n.*?0\}\},\n\n',
        '',
        content,
        flags=re.DOTALL
    )
    
    # Remove DrumBigHitThreshold page definition
    content = re.sub(
        r'    \{Menu::Page::DrumBigHitThreshold,.*?\n.*?"Big Hit\\nThreshold".*?\n.*?4095\}\},\n\n',
        '',
        content,
        flags=re.DOTALL
    )
    
    # Remove from getCurrentValue() - DrumBigHitThreshold case
    content = re.sub(
        r'    case Page::DrumBigHitThreshold:\s*\n\s*return m_store->getBigHitThreshold\(\);\s*\n',
        '',
        content
    )
    
    # Remove from getCurrentValue() - DrumBigHitEnable case
    content = re.sub(
        r'    case Page::DrumBigHitEnable:\s*\n\s*return static_cast<uint16_t>\(m_store->getBigHitEnable\(\)\);\s*\n',
        '',
        content
    )
    
    # Remove from getCurrentValue() - DrumBigHitArcade case (entire block)
    content = re.sub(
        r'    case Page::DrumBigHitArcade: \{.*?\n.*?// Return selection.*?\n.*?bool enabled.*?\n.*?uint16_t threshold.*?\n.*?if \(!enabled\).*?\n.*?return 0;.*?\n.*?\} else if \(threshold == 2000\).*?\n.*?return 1;.*?\n.*?\} else if \(threshold == 2500\).*?\n.*?return 2;.*?\n.*?\} else if \(threshold == 3000\).*?\n.*?return 3;.*?\n.*?\} else \{.*?\n.*?return 4;.*?\n.*?\}.*?\n.*?\}',
        '',
        content,
        flags=re.DOTALL
    )
    
    # Remove navigation actions
    content = re.sub(
        r'    case Descriptor::Action::GotoPageDrumBigHitArcade:\s*\n\s*gotoPage\(Page::DrumBigHitArcade\);\s*\n\s*break;\s*\n',
        '',
        content
    )
    
    content = re.sub(
        r'    case Descriptor::Action::GotoPageDrumBigHitEnable:\s*\n\s*gotoPage\(Page::DrumBigHitEnable\);\s*\n\s*break;\s*\n',
        '',
        content
    )
    
    content = re.sub(
        r'    case Descriptor::Action::GotoPageDrumBigHitThreshold:\s*\n\s*gotoPage\(Page::DrumBigHitThreshold\);\s*\n\s*break;\s*\n',
        '',
        content
    )
    
    # Remove SetBigHitEnable action (large block)
    content = re.sub(
        r'    case Descriptor::Action::SetBigHitEnable:.*?\n.*?// Handle Big Hit.*?\n.*?switch \(value\).*?\n.*?case 0:.*?\n.*?m_store->setBigHitEnable\(false\);.*?\n.*?break;.*?\n.*?case 1:.*?\n.*?m_store->setBigHitEnable\(true\);.*?\n.*?m_store->setBigHitThreshold\(2000\);.*?\n.*?break;.*?\n.*?case 2:.*?\n.*?m_store->setBigHitEnable\(true\);.*?\n.*?m_store->setBigHitThreshold\(2500\);.*?\n.*?break;.*?\n.*?case 3:.*?\n.*?m_store->setBigHitEnable\(true\);.*?\n.*?m_store->setBigHitThreshold\(3000\);.*?\n.*?break;.*?\n.*?case 4:.*?\n.*?// Don\'t change.*?\n.*?break;.*?\n.*?\}.*?\n.*?break;\s*\n',
        '',
        content,
        flags=re.DOTALL
    )
    
    # Remove SetDrumBigHitThreshold action
    content = re.sub(
        r'    case Descriptor::Action::SetDrumBigHitThreshold:\s*\n\s*m_store->setBigHitThreshold\(value\);\s*\n\s*break;\s*\n',
        '',
        content
    )
    
    # Remove from gotoParent() restore cases
    content = re.sub(
        r'        case Page::DrumBigHitThreshold:\s*\n\s*m_store->setBigHitThreshold\(current_state\.original_value\);\s*\n\s*break;\s*\n',
        '',
        content
    )
    
    content = re.sub(
        r'        case Page::DrumBigHitEnable:\s*\n\s*m_store->setBigHitEnable\(static_cast<bool>\(current_state\.original_value\)\);\s*\n\s*break;\s*\n',
        '',
        content
    )
    
    # Remove Big Hit special handling in update() Confirm button
    content = re.sub(
        r'            \} else if \(current_state\.page == Page::DrumBigHitArcade\).*?\n.*?// Apply Big Hit Mode.*?\n.*?if \(current_state\.selected_value == 4\).*?\n.*?// "Custom\.\.\.".*?\n.*?// First enable Big Hit.*?\n.*?if \(!m_store->getBigHitEnable.*?\n.*?m_store->setBigHitEnable.*?\n.*?\}.*?\n.*?performAction.*?\n.*?\} else \{.*?\n.*?// Apply the preset.*?\n.*?performAction.*?\n.*?// Update original_value.*?\n.*?current_state\.original_value.*?\n.*?// Don\'t go back.*?\n.*?\}',
        '',
        content,
        flags=re.DOTALL
    )
    
    return content

def remove_big_hit_from_settings_store_h(content):
    """Remove Big Hit functions from SettingsStore.h"""
    
    # Remove getter/setter declarations
    content = re.sub(
        r'\s*bool getB