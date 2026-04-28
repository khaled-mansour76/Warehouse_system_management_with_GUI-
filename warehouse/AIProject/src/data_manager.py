import os
import pandas as pd
from typing import Dict, List

class DataManager:
    def __init__(self, data_dir: str = "../Warehouse/data"):
        self.data_dir = data_dir
        self.product_dataframes: Dict[str, pd.DataFrame] = {}
        self.transactions_df: pd.DataFrame = pd.DataFrame()
        self.system_files = {'departments.csv', 'users.csv', 'logs.csv', 'transactions.csv'}

    def load_departments_and_discover(self) -> List[str]:
        csv_files = set()
        
        dept_path = os.path.join(self.data_dir, 'departments.csv')
        if os.path.exists(dept_path):
            try:
                dept_df = pd.read_csv(dept_path)
                if 'file_name' in dept_df.columns:
                    csv_files.update(dept_df['file_name'].dropna().tolist())
            except Exception as e:
                print(f"Error reading departments.csv: {e}")

        if os.path.exists(self.data_dir):
            all_files = os.listdir(self.data_dir)
            for f in all_files:
                if f.endswith('.csv') and f not in self.system_files:
                    csv_files.add(f)
                    
        return list(csv_files)

    def load_product_data(self) -> None:
        files_to_load = self.load_departments_and_discover()
        
        for file in files_to_load:
            file_path = os.path.join(self.data_dir, file)
            if os.path.exists(file_path):
                try:
                    df = pd.read_csv(file_path)
                    if 'id' in df.columns and 'product_id' not in df.columns:
                        df.rename(columns={'id': 'product_id'}, inplace=True)
                    self.product_dataframes[file] = df
                except Exception as e:
                    print(f"Failed to load product file {file}: {e}")

    def load_transactions(self) -> None:
        trans_path = os.path.join(self.data_dir, 'transactions.csv')
        if os.path.exists(trans_path):
            try:
                df = pd.read_csv(trans_path)
                if not df.empty and 'timestamp' in df.columns:
                    df['timestamp'] = pd.to_datetime(df['timestamp'], errors='coerce')
                self.transactions_df = df
            except Exception as e:
                print(f"Failed to load transactions.csv: {e}")

    def get_consolidated_product_data(self) -> pd.DataFrame:
        if not self.product_dataframes:
            return pd.DataFrame()
            
        dfs = []
        for file_name, df in self.product_dataframes.items():
            df_copy = df.copy()
            df_copy['source_file'] = file_name
            dfs.append(df_copy)
            
        return pd.concat(dfs, ignore_index=True)

    def get_product_transaction_history(self, product_id: str) -> pd.DataFrame:
        if self.transactions_df.empty or 'product_id' not in self.transactions_df.columns:
            return pd.DataFrame()
            
        prod_tx = self.transactions_df[self.transactions_df['product_id'] == product_id].copy()
        if prod_tx.empty:
            return pd.DataFrame()
            
        prod_tx['date'] = prod_tx['timestamp'].dt.date
        
        try:
            prod_tx['quantity'] = pd.to_numeric(prod_tx['quantity'], errors='coerce').fillna(0)
            daily_demand = prod_tx.groupby('date')['quantity'].sum().reset_index()
            daily_demand.rename(columns={'quantity': 'daily_demand'}, inplace=True)
            return daily_demand
        except Exception:
            return pd.DataFrame()
