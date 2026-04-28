import os
import sys

sys.path.append(os.path.join(os.path.dirname(__file__), 'src'))

from data_manager import DataManager
from analyzer import Analyzer
from reporter import Reporter

def main():
    print("Initializing AI Warehouse Operational Pipeline...")
    
    target_data_dir = os.path.join(os.path.dirname(__file__), '../Warehouse/data')
    
    print(" -> Loading and merging underlying CSV sources...")
    dm = DataManager(data_dir=target_data_dir)
    dm.load_product_data()
    dm.load_transactions()
    
    consolidated_df = dm.get_consolidated_product_data()
    loaded_files = list(dm.product_dataframes.keys())
    
    if consolidated_df.empty:
        print("CRITICAL: No valid product data entities integrated.")
        return

    print(" -> Executing Machine Learning & Rule Agent...")
    analyzer = Analyzer(safety_stock_threshold=20, min_history_points=3)
    analysis_results = analyzer.analyze_inventory(consolidated_df, dm)

    print(" -> Compiling Stakeholder Management Summary...\n")
    report = Reporter.generate_summary(analysis_results, loaded_files)
    
    print(report)
    
    with open("management_summary.txt", "w") as f:
        f.write(report)
        
    print("\n* Written summary document strictly preserved inside management_summary.txt")

if __name__ == "__main__":
    main()
