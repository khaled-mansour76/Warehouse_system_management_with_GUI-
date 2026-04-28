import pandas as pd
from sklearn.linear_model import LinearRegression
from typing import Dict, Any, List

class Analyzer:
    def __init__(self, safety_stock_threshold: int = 20, min_history_points: int = 3):
        self.safety_stock_threshold = safety_stock_threshold
        self.min_history_points = min_history_points

    def forecast_demand(self, product_id: str, history_df: pd.DataFrame) -> Dict[str, Any]:
        if history_df.empty or len(history_df) < self.min_history_points:
            return {"status": "insufficient_data", "predicted_demand": None}

        try:
            history_df = history_df.sort_values('date')
            X = history_df['date'].apply(lambda d: d.toordinal()).values.reshape(-1, 1)
            y = history_df['daily_demand'].values

            model = LinearRegression()
            model.fit(X, y)

            next_day_ordinal = X[-1][0] + 1
            prediction = model.predict([[next_day_ordinal]])
            
            predicted_value = max(0.0, float(prediction[0]))
            
            return {
                "status": "success",
                "predicted_demand": round(predicted_value, 2)
            }
        except Exception as e:
            return {"status": "error", "message": str(e), "predicted_demand": None}

    def check_safety_stock(self, current_quantity: float) -> bool:
        try:
            return float(current_quantity) < self.safety_stock_threshold
        except ValueError:
            return False

    def analyze_inventory(self, consolidated_products: pd.DataFrame, data_manager) -> List[Dict[str, Any]]:
        analysis_results = []

        if consolidated_products.empty:
            return analysis_results

        for _, row in consolidated_products.iterrows():
            prod_id = row.get('product_id', 'UNKNOWN')
            name = row.get('name', 'Unknown Product')
            qty = row.get('quantity', 0)
            
            result = {
                'product_id': prod_id,
                'name': name,
                'current_quantity': qty,
                'rule_flagged': self.check_safety_stock(qty),
                'forecast': None,
                'forecast_status': 'no_history'
            }

            hist_df = data_manager.get_product_transaction_history(prod_id)
            forecast_out = self.forecast_demand(prod_id, hist_df)
            
            result['forecast'] = forecast_out.get('predicted_demand')
            result['forecast_status'] = forecast_out.get('status')

            analysis_results.append(result)

        return analysis_results
