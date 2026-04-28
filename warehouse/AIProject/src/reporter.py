from typing import List, Dict, Any

class Reporter:
    @staticmethod
    def generate_summary(analysis_results: List[Dict[str, Any]], loaded_files: List[str]) -> str:
        report_lines = []
        report_lines.append("="*60)
        report_lines.append(" WAREHOUSE MANAGEMENT SYSTEM - INTELLIGENT AGENT SUMMARY ")
        report_lines.append("="*60)
        
        report_lines.append(f"\n[1] DATA INTEGRATION:")
        report_lines.append(f"     Analyzed dynamically discovered file sources ({len(loaded_files)} total):")
        for f in loaded_files:
            report_lines.append(f"       - {f}")

        report_lines.append(f"\n[2] ML DEMAND FORECASTING (Linear Regression):")
        forecasted = [r for r in analysis_results if r['forecast_status'] == 'success']
        if forecasted:
            for item in forecasted:
                report_lines.append(
                    f"     - [Product: {item['name']}, ID: {item['product_id']}] -> "
                    f"Predicted Next Day Demand: {item['forecast']} units"
                )
        else:
            report_lines.append("     - Note: No products possessed enough historical transaction context for reliable forecasting model fitting.")

        report_lines.append(f"\n[3] RULE-BASED SAFETY CHECKS (Deficit Watchlist):")
        low_stock = [r for r in analysis_results if r['rule_flagged']]
        if low_stock:
            for item in low_stock:
                report_lines.append(
                    f"     - [Product: {item['name']}, ID: {item['product_id']}] -> "
                    f"URGENT RESTOCK! Only {item['current_quantity']} active units remaining."
                )
        else:
            report_lines.append("     - Status: All items currently maintaining stable adequate volume thresholds.")

        report_lines.append("\n" + "="*60)
        
        return "\n".join(report_lines)
