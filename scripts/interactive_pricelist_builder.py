#!/usr/bin/env python3
import re
import webbrowser
import math
import os
import sys

def get_pack_info(qty_needed, price):
    print(f"\n--- Calculation Helper (Need: {qty_needed}) ---")
    
    # 1:1 Skip / Next logic
    ans = input(f"Is this a 1:1 purchase? (Just hit Enter for YES, or type 'n' to adjust): ").strip().lower()
    
    if ans == "" or ans == "y":
        return 1, 1, price, price  # 1 pack, 1 per pack, total price, used price
    
    # Pack Size (Division)
    try:
        pack_size = float(input(f"How many units are in one Amazon listing? (e.g. 120): ") or 1)
    except ValueError: pack_size = 1
    
    # Multiplier (Calculation)
    packs_to_buy = math.ceil(qty_needed / pack_size)
    print(f">> Based on Qty {qty_needed}, you need to buy: {packs_to_buy} pack(s).")
    
    confirm_buy = input(f"Buy {packs_to_buy} packs? (Enter to confirm, or type number to override): ")
    if confirm_buy.strip():
        packs_to_buy = int(confirm_buy)

    total_checkout = packs_to_buy * price
    used_value = qty_needed * (price / pack_size)
    
    return packs_to_buy, pack_size, total_checkout, used_value

def run_interactive_session(readme_path):
    if not os.path.exists(readme_path):
        print(f"Error: {readme_path} not found.")
        return

    with open(readme_path, 'r', encoding='utf-8') as f:
        content = f.read()

    # Regex to find: | Item | Qty | [Link](URL)
    table_pattern = r'\|(?:\s*[^|]*\s*)\|(?:\s*([^|]*)\s*)\|\s*(\d+)\s*\|.*\[Link\]\((https?://[^\s)]+)\)'
    matches = re.findall(table_pattern, content)
    
    results = []
    print(f"Found {len(matches)} items in your README.")
    print("Ready to start? I will open links one by one.")
    input("Press Enter to begin...")

    for i, (item_name, qty_needed, url) in enumerate(matches, 1):
        item_name = item_name.strip()
        qty_needed = int(qty_needed)
        
        print("\n" + "="*50)
        print(f"ITEM {i}/{len(matches)}: {item_name}")
        print(f"QUANTITY NEEDED: {qty_needed}")
        print(f"OPENING: {url}")
        
        webbrowser.open(url)
        
        try:
            price_input = input(f"Enter the price shown on Amazon (e.g. 12.99): ")
            price = float(re.sub(r'[^\d.]', '', price_input))
            
            buy_qty, pack_size, checkout, value = get_pack_info(qty_needed, price)
            
            results.append({
                'name': item_name,
                'url': url,
                'checkout': checkout,
                'value': value,
                'buy_qty': buy_qty
            })
        except ValueError:
            print("Skipping item due to invalid input.")

    # BUILD THE MARKDOWN FILE
    with open("OuchiTaiko_Cost_Summary.md", "w", encoding="utf-8") as md:
        md.write("# OuchiTaiko Build Cost Summary\n\n")
        md.write("| Item | Link | Buy Qty | Checkout Cost | Used Value |\n")
        md.write("| :--- | :--- | :--- | :--- | :--- |\n")
        
        total_check = 0
        total_val = 0
        
        for r in results:
            md.write(f"| {r['name']} | [Amazon Link]({r['url']}) | {r['buy_qty']} | ${r['checkout']:.2f} | ${r['value']:.2f} |\n")
            total_check += r['checkout']
            total_val += r['value']
            
        md.write(f"| **TOTAL** | | | **${total_check:.2f}** | **${total_val:.2f}** |\n\n")
        md.write(f"### Financial Breakdown\n")
        md.write(f"- **Total to Purchase:** ${total_check:.2f}\n")
        md.write(f"- **Actual Value in Drum:** ${total_val:.2f}\n")
        md.write(f"- **Spare Parts Value:** ${total_check - total_val:.2f}\n")

    print(f"\nDone! Report saved to OuchiTaiko_Cost_Summary.md")

if __name__ == "__main__":
    run_interactive_session("README.md")