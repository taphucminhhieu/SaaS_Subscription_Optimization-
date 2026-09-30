#include <stdio.h>

struct UserAccount {
    int account_id;
    int plan_type;
    int monthly_fee;
    int remaining_days;
    int active_devices;
};

int main() {
    int N;
    printf("Nhap so luong tai khoan (1 - 100): ");
    if (scanf("%d", &N) != 1 || N < 1 || N > 100) {
        printf("So luong tai khoan khong hop le!\n");
        return 1;
    }

    struct UserAccount users[100];
    int mrr_before = 0;

    for (int i = 0; i < N; i++) {
        printf("\n--- Nhap thong tin tai khoan %d ---\n", i + 1);
        printf("Ma tai khoan (ID): ");
        scanf("%d", &users[i].account_id);
        printf("Loai goi (0: Free, 1: Standard, 2: Premium): ");
        scanf("%d", &users[i].plan_type);
        printf("Phi hang thang (VNÐ): ");
        scanf("%d", &users[i].monthly_fee);
        printf("So ngay con lai: ");
        scanf("%d", &users[i].remaining_days);
        printf("So thiet bi ket noi: ");
        scanf("%d", &users[i].active_devices);

        // Chuan hoa du lieu dau vao thiet bi <= 0
        if (users[i].active_devices <= 0) {
            users[i].active_devices = 1;
        }

        // Tinh MRR truoc kiem toan (chi tinh gia tri hop le theo plan_type ban dau)
        if (users[i].plan_type > 0 && users[i].remaining_days > 0) {
            mrr_before += users[i].monthly_fee;
        }
    }

    // Kiem toan va Xu ly tu dong (Audit Rules)
    for (int i = 0; i < N; i++) {
        // 1. Quy tac het han
        if (users[i].remaining_days <= 0) {
            users[i].plan_type = 0;
            users[i].monthly_fee = 0;
        }

        // 2. Quy tac gioi han thiet bi theo goi
        if (users[i].plan_type == 0) {
            if (users[i].active_devices > 1) users[i].active_devices = 1;
            users[i].monthly_fee = 0;
        } else if (users[i].plan_type == 1) {
            if (users[i].active_devices > 2) users[i].active_devices = 2;
            users[i].monthly_fee = 120000; // Chuan hoa phi Standard
        } else if (users[i].plan_type == 2) {
            if (users[i].active_devices > 5) users[i].active_devices = 5;
            users[i].monthly_fee = 300000; // Chuan hoa phi Premium
        }
    }

    // Su dung mang phu (Auxiliary Arrays) de phan loai va sap xep on dinh (Stable Sort)
    struct UserAccount active_list[100];
    struct UserAccount free_list[100];
    int active_count = 0;
    int free_count = 0;

    for (int i = 0; i < N; i++) {
        if (users[i].plan_type > 0) {
            active_list[active_count++] = users[i];
        } else {
            free_list[free_count++] = users[i];
        }
    }

    // Sap xep active_list theo monthly_fee giam dan (Bubble Sort giu tinh on dinh)
    for (int i = 0; i < active_count - 1; i++) {
        for (int j = 0; j < active_count - i - 1; j++) {
            if (active_list[j].monthly_fee < active_list[j + 1].monthly_fee) {
                struct UserAccount temp = active_list[j];
                active_list[j] = active_list[j + 1];
                active_list[j + 1] = temp;
            }
        }
    }

    // Gop lai mang chinh (users)
    int idx = 0;
    int mrr_after = 0;
    for (int i = 0; i < active_count; i++) {
        users[idx++] = active_list[i];
        mrr_after += active_list[i].monthly_fee;
    }
    for (int i = 0; i < free_count; i++) {
        users[idx++] = free_list[i];
    }

    // Xuat bao cao
    printf("\n================================= BANG KIEM TOAN SAAS CLOUDSYNC =================================\n");
    printf("%-8s | %-10s | %-12s | %-12s | %-15s\n", "MA TK", "GOI CUOC", "PHI (VND)", "CONG LAI", "THIET BI");
    printf("-------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < N; i++) {
        printf("%-8d | %-10d | %-12d | %-12d | %-15d\n",
               users[i].account_id,
               users[i].plan_type,
               users[i].monthly_fee,
               users[i].remaining_days,
               users[i].active_devices);
    }
    printf("=================================================================================================\n");
    printf("Tong doanh thu MRR truoc khi kiem toan: %d VND\n", mrr_before);
    printf("Tong doanh thu MRR thuc te sau kiem toan: %d VND\n", mrr_after);
    printf("Chenh lech giam tru doanh thu ao: %d VND\n", mrr_before - mrr_after);

    return 0;
}
